#include "ChatServer.hpp"

ChatServer::ChatServer(QObject* parent) : QObject(parent) {
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &ChatServer::handleNewConnection);
}

bool ChatServer::start(quint16 port) {
    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Failed to start the server:" << m_tcpServer->errorString();
        return false;
    }
    qDebug() << "Server started, listening on port" << port;
    return true;
}

void ChatServer::handleNewConnection() {
    QTcpSocket* socket = m_tcpServer->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &ChatServer::handleReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatServer::handleDisconnection);
    m_clients.insert(socket);
    qDebug() << "New client connected from" << socket->peerAddress().toString();
    // addToList(socket);
    emit clientListChanged();
}

void ChatServer::broadcastClientList() {
    // Create the user list message
    QString usersList = "USERS:";
    QStringList names = m_clientNames.values();
    if (!names.isEmpty()) {
        usersList += names.join(",");
    }

    // Send to ALL connected clients
    for (QTcpSocket* clientSocket : m_clients) {
        clientSocket->write(usersList.toUtf8());
    }
    qDebug() << "Broadcasted updated user list:" << names;
}

void ChatServer::handleReadyRead() {
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    QString message = QString::fromUtf8(data);

    if (message.startsWith("NAME:")) {
        QString name = message.mid(5).trimmed();

        // Check if the name is already in use
        if (m_clientNames.values().contains(name)) {
            // Send error message if name is taken
            senderSocket->write("SYSTEM:Username already in use. Please choose another name.");
            return;
        }

        m_clientNames[senderSocket] = name; // Map the socket to the username
        qDebug() << "Client name set to" << name << "from" << senderSocket->peerAddress().toString();
        emit clientListChanged();
        // Broadcast the updated list to ALL connected clients
        broadcastClientList();
    }
    else if (message.startsWith("WIZZ:")) {
        QString targetName = message.mid(5).trimmed();
        QTcpSocket *targetSocket = getSocketByName(targetName);

        if (targetSocket) {
            QString wizzMessage = "WIZZ";
            targetSocket->write(wizzMessage.toUtf8());
            qDebug() << "Sent WIZZ command to" << targetName;
        } else {
            senderSocket->write(QString("SYSTEM:User '%1' not found").arg(targetName).toUtf8());
            qDebug() << "Target user not found: " << targetName;
        }
    }
    else if (message.startsWith("DM:")) {
        int firstColon = message.indexOf(':');
        int secondColon = message.indexOf(':', firstColon + 1);
        if (secondColon != -1) {
            QString targetName = message.mid(firstColon + 1, secondColon - firstColon - 1);
            QString dmMessage = message.mid(secondColon + 1);
            sendDirectMessage(dmMessage, targetName, senderSocket);
        }
    }
    else {
        broadcastMessage(message, senderSocket);
    }
}

void ChatServer::broadcastMessage(const QString& message, QTcpSocket* sender) {
    QString senderName = m_clientNames.value(sender, "Unknown");
    QString fullMessage = "BROADCAST:" + senderName + ": " + message;

    for (QTcpSocket* clientSocket : m_clients) {
        clientSocket->write(fullMessage.toUtf8());
    }
    qDebug() << "Broadcast message from" << senderName << ":" << message;
}

void ChatServer::sendDirectMessage(const QString& message, const QString& targetName, QTcpSocket* sender) {
    QString senderName = m_clientNames.value(sender, "Unknown");
    QTcpSocket* targetSocket = getSocketByName(targetName);

    if (targetSocket) {
        QString dmMessage = "DM:" + senderName + ": " + message;
        targetSocket->write(dmMessage.toUtf8());
        sender->write(dmMessage.toUtf8());
        qDebug() << "DM from" << senderName << "to" << targetName << ":" << message;
    } else {
        sender->write(QString("SYSTEM:User '%1' not found").arg(targetName).toUtf8());
    }
}

QTcpSocket* ChatServer::getSocketByName(const QString& name) {
    for (auto it = m_clientNames.begin(); it != m_clientNames.end(); ++it) {
        if (it.value() == name) {
            return it.key();
        }
    }
    return nullptr;
}

void ChatServer::handleDisconnection() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QString name = m_clientNames.value(socket, "Unknown");
        qDebug() << "Client" << name << "disconnected from" << socket->peerAddress().toString();

        // Notify remaining clients
        QString disconnectMessage = "SYSTEM:" + name + " has disconnected";
        for (QTcpSocket* clientSocket : m_clients) {
            if (clientSocket != socket) {
                clientSocket->write(disconnectMessage.toUtf8());
            }
        }

        // Clean up socket data
        m_clientNames.remove(socket);
        m_clients.remove(socket);

        // Ensure proper cleanup
        socket->close();
        socket->deleteLater(); // Schedule for deletion

        emit clientListChanged();
        broadcastClientList();
    }
}


// void ChatServer::addToList(QTcpSocket* socket) {
//     QString name = m_clientNames.value(socket, "Unknown");
//     m_clientNamesList.append(name);
// }

// void ChatServer::removeFromList(QTcpSocket* socket) {
//     QString name = m_clientNames.value(socket, "Unknown");
//     m_clientNamesList.removeOne(name);
// }

QList<QString> ChatServer::getConnectedClients() {
    QList<QString> clientList = m_clientNames.values(); // Directly get usernames
    qDebug() << "Connected Clients:" << clientList;
    return clientList;
}


void ChatServer::disconnectClientByName(const QString &clientName) {
    QTcpSocket* clientSocket = getSocketByName(clientName);

    if (clientSocket) {
        qDebug() << "Disconnecting client:" << clientName;

        // Initiate disconnection
        clientSocket->disconnectFromHost();
    } else {
        qDebug() << "Client" << clientName << "not found on the server.";
    }
}


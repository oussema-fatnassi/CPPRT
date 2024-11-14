#include "ChatServer.hpp"
#include <QDebug>
#include <QTcpSocket>

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
}

void ChatServer::handleReadyRead() {
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (senderSocket) {
        QByteArray data = senderSocket->readAll();
        QString message = QString::fromUtf8(data);

        if (message.startsWith("NAME:")) {
            QString name = message.mid(5).trimmed();
            m_clientNames[senderSocket] = name;
            qDebug() << "Client name set to" << name << "from" << senderSocket->peerAddress().toString();
        } else {
            QString senderName = m_clientNames.value(senderSocket, "Unknown");
            QString fullMessage = senderName + ": " + message;

            for (QTcpSocket* clientSocket : m_clients) {
                clientSocket->write(fullMessage.toUtf8());
            }
            qDebug() << "Broadcast message from" << senderName << ":" << message;
        }
    }
}



void ChatServer::handleDisconnection() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QString name = m_clientNames.value(socket, "Unknown");
        qDebug() << "Client" << name << "disconnected from" << socket->peerAddress().toString();
        m_clientNames.remove(socket);
        m_clients.remove(socket);
        socket->deleteLater();
    }
}

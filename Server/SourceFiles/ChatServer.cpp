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
    qDebug() << "New client connected from" << socket->peerAddress().toString();
}

void ChatServer::handleReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QByteArray data = socket->readAll();
        QString message = QString::fromUtf8(data);

        // Check if the message is a name setup message
        if (message.startsWith("NAME:")) {
            QString name = message.mid(5).trimmed();
            m_clientNames[socket] = name;
            qDebug() << "Client name set to" << name << "from" << socket->peerAddress().toString();
        } else {
            QString senderName = m_clientNames.value(socket, "Unknown");
            qDebug() << "Message from" << senderName << "(" << socket->peerAddress().toString() << "):" << message;
            socket->write(data); // Echo back the message
        }
    }
}

void ChatServer::handleDisconnection() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QString name = m_clientNames.value(socket, "Unknown");
        qDebug() << "Client" << name << "disconnected from" << socket->peerAddress().toString();
        m_clientNames.remove(socket);
        socket->deleteLater();
    }
}

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
        qDebug() << "Received data from client:" << data;

        socket->write(data);
    }
}

void ChatServer::handleDisconnection() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        qDebug() << "Client disconnected from" << socket->peerAddress().toString();
        socket->deleteLater();
    }
}

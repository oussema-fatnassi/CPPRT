#include "ChatClient.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>

ChatClient::ChatClient(QObject* parent) : QObject(parent) {
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &ChatClient::handleConnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ChatClient::handleReadyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ChatClient::handleDisconnected);
}

void ChatClient::connectToServer(const QHostAddress& host, quint16 port) {
    qDebug() << "Attempting to connect to server at" << host.toString() << "on port" << port;
    m_tcpSocket->connectToHost(host, port);
    if (m_tcpSocket->waitForConnected(3000)) {
        qDebug() << "Connection established successfully.";
    } else {
        qDebug() << "Connection failed:" << m_tcpSocket->errorString();
    }
}

void ChatClient::sendMessage(const QString& message) {
    if (m_tcpSocket->state() == QTcpSocket::ConnectedState) {
        m_tcpSocket->write(message.toUtf8());
    } else {
        qDebug() << "Not connected to the server";
    }
}

void ChatClient::handleConnected() {
    qDebug() << "Connected to server";
}

void ChatClient::handleReadyRead() {
    QByteArray data = m_tcpSocket->readAll();
    qDebug() << "Received echo from server:" << data;
}

void ChatClient::handleDisconnected() {
    qDebug() << "Disconnected from server";
}

bool ChatClient::isConnected() const {
    return m_tcpSocket->state() == QTcpSocket::ConnectedState;
}

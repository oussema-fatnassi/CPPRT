#include "ChatClient.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>

ChatClient::ChatClient(const QString &name, QObject* parent)
    : QObject(parent), m_name(name) {
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &ChatClient::handleConnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ChatClient::handleReadyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ChatClient::handleDisconnected);
}

bool ChatClient::connectToServer(const QHostAddress& host, quint16 port) {
    qDebug() << "Attempting to connect to server at" << host.toString() << "on port" << port;

    // Check if socket is valid
    if (!m_tcpSocket) {
        qDebug() << "TCP Socket is null";
        return false;
    }

    m_tcpSocket->connectToHost(host, port);

    // Wait for connection with a timeout
    if (m_tcpSocket->waitForConnected(5000)) { // Increased timeout to 5 seconds
        qDebug() << "Connection established successfully.";
        m_tcpSocket->write(("NAME:" + m_name).toUtf8());
        return true;  // Connection successful
    } else {
        // Log the specific error
        qDebug() << "Connection failed:" << m_tcpSocket->errorString();
        qDebug() << "Socket state:" << m_tcpSocket->state();
        return false;  // Connection failed
    }
}

void ChatClient::sendMessage(const QString& message) {
    if (m_tcpSocket->state() == QTcpSocket::ConnectedState) {
        m_tcpSocket->write(message.toUtf8());
    } else {
        qDebug() << "Not connected to the server";
    }
}

void ChatClient::sendDirectMessage(const QString& targetName, const QString& message) {
    if (m_tcpSocket->state() == QTcpSocket::ConnectedState) {
        QString dmMessage = "DM:" + targetName + ":" + message;
        m_tcpSocket->write(dmMessage.toUtf8());
    } else {
        qDebug() << "Not connected to the server";
    }
}

void ChatClient::handleConnected() {
    qDebug() << "Connected to server";
}

void ChatClient::handleReadyRead() {
    QByteArray data = m_tcpSocket->readAll();
    QString message = QString::fromUtf8(data);

    if (message.startsWith("USERS:")) {
        // Update online users list
        m_onlineUsers = message.mid(6).split(",", Qt::SkipEmptyParts);
        qDebug() << "Online users:" << m_onlineUsers;
    }
    else if (message.startsWith("BROADCAST:")) {
        QString actualMessage = message.mid(10);
        if (actualMessage.startsWith(m_name + ":")) {
            actualMessage.replace(0, m_name.length(), "You");
        }
        qDebug() << "[Broadcast]" << actualMessage;
    }
    else if (message.startsWith("DM:")) {
        QString dmMessage = message.mid(3);
        if (dmMessage.startsWith(m_name + ":")) {
            dmMessage.replace(0, m_name.length(), "You");
        }
        qDebug() << "[Private]" << dmMessage;
    }
    else if (message.startsWith("SYSTEM:")) {
        qDebug() << "[System]" << message.mid(7);
    }
}

void ChatClient::handleDisconnected() {
    qDebug() << "Disconnected from server";
    m_onlineUsers.clear();
}

bool ChatClient::isConnected() const {
    return m_tcpSocket->state() == QTcpSocket::ConnectedState;
}

QStringList ChatClient::getOnlineUsers() const {
    return m_onlineUsers;
}

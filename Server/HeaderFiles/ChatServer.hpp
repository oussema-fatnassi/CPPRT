#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSet>

class ChatServer : public QObject {
    Q_OBJECT

public:
    ChatServer(QObject* parent = nullptr);
    bool start(quint16 port);

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnection();

private:
    QTcpServer* m_tcpServer;
    QMap<QTcpSocket*, QString> m_clientNames;
    QSet<QTcpSocket*> m_clients;
};

#endif // CHAT_SERVER_H

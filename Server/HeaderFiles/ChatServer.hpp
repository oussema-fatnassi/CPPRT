#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <QObject>
#include <QTcpServer>

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
};

#endif // CHAT_SERVER_H

#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QSet>
#include <QMap>

class ChatServer : public QObject {
    Q_OBJECT
public:
    ChatServer(QObject* parent = nullptr);
    bool start(quint16 port);

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnection();
    void broadcastClientList();

private:
    void broadcastMessage(const QString& message, QTcpSocket* sender);
    void sendDirectMessage(const QString& message, const QString& targetName, QTcpSocket* sender);
    QTcpSocket* getSocketByName(const QString& name);

    QTcpServer* m_tcpServer;
    QMap<QTcpSocket*, QString> m_clientNames;
    QSet<QTcpSocket*> m_clients;
};
#endif // CHAT_SERVER_H

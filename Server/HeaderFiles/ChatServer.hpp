#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QSet>
#include <QMap>
#include <QDebug>
#include <QTcpSocket>

class QTcpSocket;
class ChatServer : public QObject {
    Q_OBJECT
    friend class MainWindow;

signals:
    void clientListChanged();

public:
    ChatServer(QObject* parent = nullptr);
    void disconnectClientByName(const QString &clientName);

    bool start(quint16 port);

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnection();
    void broadcastClientList();
    // void addToList(QTcpSocket* socket);
    // void removeFromList(QTcpSocket* socket);
    QList<QString> getConnectedClients();

private:
    void broadcastMessage(const QString& message, QTcpSocket* sender);
    void sendDirectMessage(const QString& message, const QString& targetName, QTcpSocket* sender);
    QTcpSocket* getSocketByName(const QString& name);
    

    QTcpServer* m_tcpServer;
    QMap<QTcpSocket*, QString> m_clientNames;
    QSet<QTcpSocket*> m_clients;
    QList<QString> m_clientNamesList;
};
#endif // CHAT_SERVER_H

/**
 * ChatServer class
 * This class is responsible for handling the server side of the chat application.
 * It listens for incoming connections, sends messages, and keeps track of the connected clients.
 * It also broadcasts messages to all clients and sends direct messages to specific clients.
 */

#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QSet>
#include <QMap>
#include <QDebug>
#include <QTcpSocket>

class ChatServer : public QObject {
    Q_OBJECT
    friend class MainWindow;

// Public methods are used to start the server and disconnect a client by name.
signals:
    void clientListChanged();
    void generalMessageReceived(const QString& message);

// Public methods are used to start the server and disconnect a client by name.
public:
    ChatServer(QObject* parent = nullptr);
    void disconnectClientByName(const QString &clientName);
    bool start(quint16 port);

// Private slots are used to handle new connections, incoming messages, and disconnections.
private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnection();
    void broadcastClientList();
    QList<QString> getConnectedClients();

// Private methods are used to broadcast messages, send direct messages, and get a socket by name.
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

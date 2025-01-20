/*
* This class is responsible for handling the client side of the chat application.
* It connects to the server, sends messages, and receives messages.
* It also keeps track of the online users.
*/

#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
#include <QObject>
#include <QHostAddress>
#include <QStringList>

class QTcpSocket;

class ChatClient : public QObject {
    Q_OBJECT

// Public methods are used to connect to the server, send messages, and get the online users.
public:
    ChatClient(const QString &name, QObject* parent = nullptr);
    bool connectToServer(const QHostAddress& host, quint16 port);
    void sendMessage(const QString& message);
    void sendDirectMessage(const QString& targetName, const QString& message);
    bool isConnected() const;
    QStringList getOnlineUsers() const;
    QString getName() const { return m_name; }

// Slots are used to handle the connection, messages, and disconnection.
private slots:
    void handleConnected();
    void handleReadyRead();
    void handleDisconnected();

// Private methods are used to handle the messages received from the server.
private:
    QTcpSocket* m_tcpSocket;
    QString m_name;
    QStringList m_onlineUsers;

// Signals are used to notify the GUI when a message is received, the user list is updated, or the connection is lost.
signals:
    void messageReceived(const QString& message);
    void userListUpdated(const QStringList& users);
    void nameRejected(const QString& reason);
    void privateMessageReceived(const QString& sender, const QString& recipient, const QString& message);
    void wizzReceived();
    void disconnected();

};
#endif // CHAT_CLIENT_H

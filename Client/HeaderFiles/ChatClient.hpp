#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>

class ChatClient : public QObject {
    Q_OBJECT
public:
    ChatClient(const QString &name, QObject* parent = nullptr);
    bool connectToServer(const QHostAddress& host, quint16 port);
    void sendMessage(const QString& message);
    void sendDirectMessage(const QString& targetName, const QString& message);
    bool isConnected() const;
    QStringList getOnlineUsers() const;
    void sendWizz(const QString& targetName);
    QString getName() const { return m_name; }

signals:
    void messageReceived(const QString& message);
    void userListUpdated(const QStringList& users);
    void disconnected();  // Signal emitted when the client disconnects
    void wizzReceived(const QString& sender); // Signal emitted when a wizz is received
private slots:
    void handleConnected();
    void handleReadyRead();
    void handleDisconnected();

private:
    QTcpSocket* m_tcpSocket;
    QString m_name;
    QStringList m_onlineUsers;
};

#endif // CHAT_CLIENT_H
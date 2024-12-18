#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H
#include <QObject>
#include <QHostAddress>
#include <QStringList>

class QTcpSocket;

class ChatClient : public QObject {
    Q_OBJECT

signals:
    void wizzReceived();
    
public:
    ChatClient(const QString &name, QObject* parent = nullptr);
    bool connectToServer(const QHostAddress& host, quint16 port);
    void sendMessage(const QString& message);
    void sendDirectMessage(const QString& targetName, const QString& message);
    bool isConnected() const;
    QStringList getOnlineUsers() const;

private slots:
    void handleConnected();
    void handleReadyRead();
    void handleDisconnected();

private:
    QTcpSocket* m_tcpSocket;
    QString m_name;
    QStringList m_onlineUsers;

signals:
    void messageReceived(const QString& message);
    void userListUpdated(const QStringList& users);

};
#endif // CHAT_CLIENT_H

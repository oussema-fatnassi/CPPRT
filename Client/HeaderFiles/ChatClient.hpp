#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <QObject>
#include <QHostAddress>

class QTcpSocket;

class ChatClient : public QObject {
    Q_OBJECT

public:
    ChatClient(QObject* parent = nullptr);
    void connectToServer(const QHostAddress& host, quint16 port);
    void sendMessage(const QString& message);
    bool isConnected() const;

private slots:
    void handleConnected();
    void handleReadyRead();
    void handleDisconnected();

private:
    QTcpSocket* m_tcpSocket;
};

#endif // CHAT_CLIENT_H

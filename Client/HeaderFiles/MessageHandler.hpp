/**
 * Class MessageHandler
 * This class is used to handle the messages in the chat application.
 * It formats the messages and adds them to the general channel and tabs.
 * It also formats the messages with the sender's name.
 */

#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <QObject>
#include <QStandardItemModel>
#include <QListView>
#include <QTime>

class MessageHandler : public QObject {
    Q_OBJECT

// Public methods are used to set the general channel model and add messages to the general channel and tabs.
public:
    explicit MessageHandler(QObject* parent = nullptr);
    void setGeneralChannelModel(QStandardItemModel* model);
    void addMessageToGeneral(const QString& message , bool isLocalMessage);
    void addMessageToTab(QListView* listView, const QString& message, bool isLocalMessage = false);
    QString formatMessage(const QString& message, const QString& sender = QString());

// Private methods are used to format the messages with the sender's name.
private:
    QStandardItemModel* generalChannelModel;
};

#endif // MESSAGEHANDLER_HPP

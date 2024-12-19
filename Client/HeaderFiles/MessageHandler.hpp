#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP


#include <QObject>
#include <QStandardItemModel>
#include <QListView>
#include <QTime>

class MessageHandler : public QObject {
    Q_OBJECT

public:
    explicit MessageHandler(QObject* parent = nullptr);
    void setGeneralChannelModel(QStandardItemModel* model);
    void addMessageToGeneral(const QString& message);
    void addMessageToTab(QListView* listView, const QString& message, bool isLocalMessage = false);
    QString formatMessage(const QString& message, const QString& sender = QString());

private:
    QStandardItemModel* generalChannelModel;
};

#endif // MESSAGEHANDLER_HPP

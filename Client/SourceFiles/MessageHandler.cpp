#include "MessageHandler.hpp"

MessageHandler::MessageHandler(QObject* parent)
    : QObject(parent), generalChannelModel(nullptr) {}

void MessageHandler::setGeneralChannelModel(QStandardItemModel* model) {
    generalChannelModel = model;
}

void MessageHandler::addMessageToGeneral(const QString& message) {
    if (!generalChannelModel) return;

    QString formattedMessage = formatMessage(message);
    QStandardItem* item = new QStandardItem(formattedMessage);
    generalChannelModel->appendRow(item);
}

void MessageHandler::addMessageToTab(QListView* listView, const QString& message, bool isLocalMessage) {
    if (!listView) return;

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(listView->model());
    if (!model) {
        model = new QStandardItemModel(listView);
        listView->setModel(model);
    }

    QStandardItem* item = new QStandardItem(message);
    item->setData(isLocalMessage, Qt::UserRole);
    model->appendRow(item);
    listView->scrollToBottom();
}

QString MessageHandler::formatMessage(const QString& message, const QString& sender) {
    QString timestamp = QTime::currentTime().toString("hh:mm:ss");
    if (sender.isEmpty()) {
        return QString("[%1] %2").arg(timestamp).arg(message);
    }
    return QString("[%1] %2: %3").arg(timestamp).arg(sender).arg(message);
}

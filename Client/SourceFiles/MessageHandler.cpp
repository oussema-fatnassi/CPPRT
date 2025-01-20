#include "MessageHandler.hpp"
#include "MessageBubbleManager.hpp"

// Constructor for the MessageHandler class
MessageHandler::MessageHandler(QObject* parent)
    : QObject(parent), generalChannelModel(nullptr) {}

// Set the general channel model
void MessageHandler::setGeneralChannelModel(QStandardItemModel* model) {
    generalChannelModel = model;
}

// Add a message to the general channel with the specified sender
void MessageHandler::addMessageToGeneral(const QString& message , bool isLocalMessage) {
    if (generalChannelModel) {
        QStandardItem* item = new QStandardItem(message);
        item->setData(isLocalMessage, Qt::UserRole + 1);
        item->setData(QTime::currentTime().toString("hh:mm"), Qt::UserRole + 2);
        generalChannelModel->appendRow(item);
    }
}

// Add a message to the specified tab with the specified sender
void MessageHandler::addMessageToTab(QListView* listView, const QString& message, bool isLocalMessage) {
    if (!listView)
        return;

    // Get the model from the list view
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(listView->model());
    if (!model) {
        model = new QStandardItemModel(listView);
        listView->setModel(model);

        // Set up the MessageBubbleManager for this list view
        MessageBubbleManager* delegate = new MessageBubbleManager(listView);
        listView->setItemDelegate(delegate);

        // Configure the list view
        listView->setSpacing(12); // Ensure enough spacing between items
        listView->setUniformItemSizes(false); // Allow different item sizes
        listView->setWordWrap(true); // Enable word wrapping
    }

    // Add the message item
    QStandardItem* item = new QStandardItem(message);
    item->setData(isLocalMessage, Qt::UserRole + 1);
    item->setData(QTime::currentTime().toString("hh:mm"), Qt::UserRole + 2);
    model->appendRow(item);

    // Scroll to the bottom
    listView->scrollToBottom();
}

// Format the message with the specified sender
QString MessageHandler::formatMessage(const QString& message, const QString& sender) {
    if (sender.isEmpty()) {
        return message;
    }
    return QString("%1: %2").arg(sender, message);
}

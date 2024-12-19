#include "UIEventHandler.hpp"

UIEventHandler::UIEventHandler(QTextEdit* textEdit, QPushButton* sendButton,
                               QListWidget* clientsList, ChatClient* chatClient,
                               TabManager* tabManager, MessageHandler* messageHandler,
                               QObject* parent)
    : QObject(parent), textEdit(textEdit), sendButton(sendButton),
    clientsList(clientsList), chatClient(chatClient),
    tabManager(tabManager), messageHandler(messageHandler) {}

bool UIEventHandler::handleKeyPress(QObject* obj, QEvent* event) {
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) &&
            keyEvent->modifiers() & Qt::ShiftModifier) {
            QTextCursor cursor = textEdit->textCursor();
            cursor.insertText("\n");
            return true;
        }

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            onSendClicked();
            return true;
        }
    }
    return false;
}

void UIEventHandler::onSendClicked() {
    QString message = textEdit->toPlainText();
    if (message.isEmpty()) return;

    QString currentTab = tabManager->getCurrentTabName();

    if (currentTab == "General Channel") {
        if (chatClient) {
            chatClient->sendMessage(message);
            messageHandler->addMessageToGeneral(message);
        }
    } else {
        if (chatClient) {
            chatClient->sendDirectMessage(currentTab, message);
            QListView* listView = tabManager->getOrCreateTab(currentTab);
            QString formattedMessage = messageHandler->formatMessage(message, "You");
            messageHandler->addMessageToTab(listView, formattedMessage, true);
        }
    }

    textEdit->clear();
}

void UIEventHandler::onUserDoubleClicked(QListWidgetItem* item) {
    QString username = item->text();
    tabManager->getOrCreateTab(username);
}

void UIEventHandler::updateUserList(const QStringList& users) {
    clientsList->clear();
    for (const QString& user : users) {
        clientsList->addItem(user);
    }
}

void UIEventHandler::setChatClient(ChatClient* chatClient) {
    this->chatClient = chatClient;
    qDebug() << "ChatClient set in UIEventHandler!";
}

bool UIEventHandler::hasChatClient() const {
    return this->chatClient != nullptr;
}

#include "UIEventHandler.hpp"

// Constructor for the UIEventHandler class
UIEventHandler::UIEventHandler(QTextEdit* textEdit, QPushButton* sendButton,
                               QListWidget* clientsList, ChatClient* chatClient,
                               TabManager* tabManager, MessageHandler* messageHandler,
                               QObject* parent)
    : QObject(parent), textEdit(textEdit), sendButton(sendButton),
    clientsList(clientsList), chatClient(chatClient),
    tabManager(tabManager), messageHandler(messageHandler) {}

// Handle the key press event
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

// Handle the send button click event
void UIEventHandler::onSendClicked() {
    QString message = textEdit->toPlainText();
    if (message.isEmpty()) return;

    QString currentTab = tabManager->getCurrentTabName();

    if (currentTab == "General Channel") {
        if (chatClient) {
            chatClient->sendMessage(message);
            messageHandler->addMessageToGeneral(message, true);
        }
    } else {
        if (chatClient) {
            chatClient->sendDirectMessage(currentTab, message);
        }
    }

    textEdit->clear();
}

// Handle the user double click event
void UIEventHandler::onUserDoubleClicked(QListWidgetItem* item) {
    QString username = item->text();
    tabManager->getOrCreateTab(username);
}

// Update the user list in the chat application
void UIEventHandler::updateUserList(const QStringList& users) {
    clientsList->clear();
    for (const QString& user : users) {
        clientsList->addItem(user);
    }
}

// Set the chat client
void UIEventHandler::setChatClient(ChatClient* chatClient) {
    this->chatClient = chatClient;
    qDebug() << "ChatClient set in UIEventHandler!";
}

// Check if the chat client exists
bool UIEventHandler::hasChatClient() const {
    return this->chatClient != nullptr;
}

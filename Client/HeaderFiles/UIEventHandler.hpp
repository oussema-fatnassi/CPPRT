/**
 * UIEventHandler class
 * This class is responsible for handling the UI events.
 * It handles the key press events, the send button click event, and the user double click event.
 * It also updates the user list in the chat application.
 */

#ifndef UIEVENTHANDLER_HPP
#define UIEVENTHANDLER_HPP

#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>
#include <QPushButton>
#include <QListWidget>
#include "ChatClient.hpp"
#include "TabManager.hpp"
#include "MessageHandler.hpp"

class UIEventHandler : public QObject {
    Q_OBJECT

// Public methods are used to handle the key press events, the send button click event, and the user double click event.
public:
    explicit UIEventHandler(QTextEdit* textEdit, QPushButton* sendButton,
                            QListWidget* clientsList, ChatClient* chatClient,
                            TabManager* tabManager, MessageHandler* messageHandler,
                            QObject* parent = nullptr);
    bool handleKeyPress(QObject* obj, QEvent* event);
    void setChatClient(ChatClient* chatClient);
    bool hasChatClient() const;

// Slots are used to handle the send button click event and the user double click event.
public slots:
    void onSendClicked();
    void onUserDoubleClicked(QListWidgetItem* item);
    void updateUserList(const QStringList& users);

// Private methods are used to handle the key press events.
private:
    QTextEdit* textEdit;
    QPushButton* sendButton;
    QListWidget* clientsList;
    ChatClient* chatClient;
    TabManager* tabManager;
    MessageHandler* messageHandler;
};

#endif UIEVENTHANDLER_HPP

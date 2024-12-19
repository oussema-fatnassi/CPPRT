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

public:
    explicit UIEventHandler(QTextEdit* textEdit, QPushButton* sendButton,
                            QListWidget* clientsList, ChatClient* chatClient,
                            TabManager* tabManager, MessageHandler* messageHandler,
                            QObject* parent = nullptr);
    bool handleKeyPress(QObject* obj, QEvent* event);
    void setChatClient(ChatClient* chatClient);
    bool hasChatClient() const;


public slots:
    void onSendClicked();
    void onUserDoubleClicked(QListWidgetItem* item);
    void updateUserList(const QStringList& users);

private:
    QTextEdit* textEdit;
    QPushButton* sendButton;
    QListWidget* clientsList;
    ChatClient* chatClient;
    TabManager* tabManager;
    MessageHandler* messageHandler;
};

#endif UIEVENTHANDLER_HPP

#include "WizzManager.hpp"
#include <QDebug>

WizzManager::WizzManager(QListWidget* clientsList, ChatClient* chatClient,
                         QWidget* effectParent, QObject* parent)
    : QObject(parent), clientsList(clientsList), chatClient(chatClient),
    effectParent(effectParent) {}

void WizzManager::onWizzButtonClicked() {
    QList<QListWidgetItem*> selectedItems = clientsList->selectedItems();
    if (!selectedItems.isEmpty()) {
        QString targetName = selectedItems.first()->text();
        QString wizzCommand = "WIZZ:" + targetName;

        if (chatClient) {
            qDebug() << "Sending WIZZ command to" << targetName;
            chatClient->sendMessage(wizzCommand);
        } else {
            qDebug() << "Chat client not initialized!";
        }
    } else {
        qDebug() << "No user selected!";
    }
}

void WizzManager::triggerWizzEffect() {
    qDebug() << "Wizz effect triggered!";
    WizzEffect* wizzEffect = new WizzEffect(effectParent);
    wizzEffect->start();
}

void WizzManager::setChatClient(ChatClient* chatClient) {
    this->chatClient = chatClient;
    qDebug() << "ChatClient set in WizzManager!";
}

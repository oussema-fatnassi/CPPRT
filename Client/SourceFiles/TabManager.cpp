#include "TabManager.hpp"
#include <QVBoxLayout>
#include "MessageBubbleManager.hpp"

TabManager::TabManager(QTabWidget* tabWidget, MessageHandler* messageHandler, QObject* parent)
    : QObject(parent), tabWidget(tabWidget), messageHandler(messageHandler) {}

QListView* TabManager::getOrCreateTab(const QString& username) {
    // Check if tab already exists
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == username) {
            QWidget* tab = tabWidget->widget(i);
            return tab->findChild<QListView*>();
        }
    }

    return createNewTab(username);
}

QListView* TabManager::createNewTab(const QString& username) {
    QWidget* newTab = new QWidget(tabWidget);
    QListView* listView = new QListView(newTab);

    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setWrapping(false);
    listView->setWordWrap(true);
    listView->setSpacing(12);
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    listView->setStyleSheet(
        "QListView {"
        "    background-color: #2b2b2b;"
        "    color: #ffffff;"
        "    border: 1px solid #555;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 14px;"
        "    font-family: Arial, sans-serif;"
        "}"
        "QListView::item {"
        "    background-color: #2b2b2b;"
        "    color: #ffffff;"
        "    padding: 5px;"
        "    border-bottom: 1px solid #444;"
        "}"
        "QListView::item:hover {"
        "    background-color: #3c3c3c;"
        "    color: #ffffff;"
        "}"
        "QListView::item:selected {"
        "    background-color: #555;"
        "    color: #ffffff;"
        "}"
        "QListView::item:selected:active {"
        "    background-color: #666;"
        "    color: #ffffff;"
        "}"
        );

    QVBoxLayout* layout = new QVBoxLayout(newTab);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(listView);
    newTab->setLayout(layout);

    // Attach the bubble delegate
    MessageBubbleManager* delegate = new MessageBubbleManager(listView);
    listView->setItemDelegate(delegate);

    tabWidget->addTab(newTab, username);
    tabWidget->setCurrentWidget(newTab);

    return listView;
}


bool TabManager::hasTab(const QString& username) const {
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == username) {
            return true;
        }
    }
    return false;
}

QString TabManager::getCurrentTabName() const {
    return tabWidget->tabText(tabWidget->currentIndex());
}

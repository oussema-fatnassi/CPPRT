#include "TabManager.hpp"
#include <QVBoxLayout>

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
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout(newTab);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(listView);
    newTab->setLayout(layout);

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

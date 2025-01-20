/**
 * Class TabManager
 * This class is used to manage the tabs in the chat application.
 * It creates new tabs for each user and keeps track of the current tab.
 * It also checks if a tab exists for a given user.
 */

#ifndef TABMANAGER_HPP
#define TABMANAGER_HPP

#include <QObject>
#include <QTabWidget>
#include <QListView>
#include <QString>
#include "MessageHandler.hpp"

class TabManager : public QObject {
    Q_OBJECT

// Public methods are used to get or create a tab for a given user and check if a tab exists for a given user.
public:
    explicit TabManager(QTabWidget* tabWidget, MessageHandler* messageHandler, QObject* parent = nullptr);
    QListView* getOrCreateTab(const QString& username);
    bool hasTab(const QString& username) const;
    QString getCurrentTabName() const;

// Private methods are used to create a new tab for a given user.
private:
    QTabWidget* tabWidget;
    MessageHandler* messageHandler;
    QListView* createNewTab(const QString& username);
};

#endif // TABMANAGER_HPP

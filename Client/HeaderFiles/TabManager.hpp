#ifndef TABMANAGER_HPP
#define TABMANAGER_HPP

#include <QObject>
#include <QTabWidget>
#include <QListView>
#include <QString>
#include "MessageHandler.hpp"

class TabManager : public QObject {
    Q_OBJECT

public:
    explicit TabManager(QTabWidget* tabWidget, MessageHandler* messageHandler, QObject* parent = nullptr);
    QListView* getOrCreateTab(const QString& username);
    bool hasTab(const QString& username) const;
    QString getCurrentTabName() const;

private:
    QTabWidget* tabWidget;
    MessageHandler* messageHandler;
    QListView* createNewTab(const QString& username);
};

#endif // TABMANAGER_HPP

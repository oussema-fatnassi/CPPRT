#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <QTextBrowser>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QString>
#include <QMap>

class UIManager {
public:
    static void setupTextBrowser(QTextBrowser* textBrowser);
    static void setupTextEdit(QTextEdit* textEdit);
    static void setupButtons(QPushButton* buttonSend, QPushButton* buttonWizz);
    static void setupUserList(QListWidget* listClients);
    static QString formatMessage(const QString& message, bool isSentByMe);
    static QString replaceEmojis(QString message);
};

#endif // UI_MANAGER_HPP
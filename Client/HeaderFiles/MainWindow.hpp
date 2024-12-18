#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTextBrowser>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QKeyEvent>
#include <QScrollBar>
#include "ChatClient.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setChatClient(ChatClient* client);

private slots:
    void onBtnSendClicked();
    void onPushButtonWizzClicked();
    void onMessageReceived(const QString& message);
    void updateUserList(const QStringList& users);
    void handleServerDisconnection();

private:
    void setupUi();
    void setupEmojiButton();
    void setupConnections();
    void addEmojiToMessage(const QString& emoji);
    QString formatMessage(const QString& message, bool isSentByMe);
    QString replaceEmojis(QString message);
    
    ChatClient* chatClient;
    QTextBrowser* textBrowser;
    QListWidget* listClients;
    QTextEdit* textEdit;
    QPushButton* buttonSend;
    QPushButton* buttonWizz;
    QPushButton* emojiButton;
    QMenu* emojiMenu;

    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_HPP
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFrame>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextBrowser>
#include <QTextEdit>
#include <QKeyEvent>
#include <QMenu>
#include "ChatClient.hpp"
#include "UIManager.hpp"

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
    void onWizzReceived(const QString& sender);

private:
    void setupUi();
    void setupConnections();
    void setupEmojiButton();
    void addEmojiToMessage(const QString& emoji);

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
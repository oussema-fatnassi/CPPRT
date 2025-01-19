#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "ChatClient.hpp"
#include "MessageHandler.hpp"
#include "TabManager.hpp"
#include "UIEventHandler.hpp"
#include "WizzManager.hpp"
#include <QMessageBox>
#include "EmojiManager.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void setChatClient(ChatClient* client);

private slots:
    void onGeneralMessageReceived(const QString& message);
    void onPrivateMessageReceived(const QString& senderName, const QString& recipient, const QString& dmContent);
    void handleServerDisconnection();

private:
    void setupMainWindowUI();
    bool eventFilter(QObject* obj, QEvent* event) override;

    Ui::MainWindow* ui;
    ChatClient* chatClient;
    MessageHandler* messageHandler;
    TabManager* tabManager;
    EmojiManager* emojiManager;
    UIEventHandler* uiEventHandler;
    WizzManager* wizzManager;
    QStandardItemModel* listViewModel;
};

#endif // MAINWINDOW_HPP

/**
 * MainWindow class header file.
 * This class is responsible for the main window of the chat application.
 * It handles the UI, the chat client, the message handler, the tab manager, the UI event handler, and the wizz manager.
 * It also handles the general and private messages received from the server.
 */

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

// Public methods are used to set the chat client and handle the general and private messages received from the server.
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void setChatClient(ChatClient* client);

// Slots are used to handle the general and private messages received from the server and the server disconnection.
private slots:
    void onGeneralMessageReceived(const QString& message);
    void onPrivateMessageReceived(const QString& senderName, const QString& recipient, const QString& dmContent);
    void handleServerDisconnection();

// Private methods are used to set up the main window UI and handle the events.
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

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
#include <QStandardItemModel>
#include <QKeyEvent>
#include "ChatClient.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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
    void setupMainWindowUI();
    ChatClient *chatClient;

    Ui::MainWindow *ui;
    QStandardItemModel *listViewModel;
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_HPP

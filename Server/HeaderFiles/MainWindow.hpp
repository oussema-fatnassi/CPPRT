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
#include <QCheckBox>
#include <QTabWidget>
#include <QTcpSocket>
#include "ChatServer.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ChatServer* server, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDisconnectButtonClicked();
    void onDisconnectAllButtonClicked();
    void onSendMessageTab1Clicked();
    void onSendMessageTab2Clicked();

private:
    Ui::MainWindow *ui;

    ChatServer* m_chatServer; // Declaration of the member for the chat server

    void setupConnections();
};

#endif // MAINWINDOW_HPP

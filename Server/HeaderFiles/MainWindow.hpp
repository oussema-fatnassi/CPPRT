/**
 * Main window class
 * This class is responsible for the main window of the chat application.
 * It handles the UI, the chat server, and the events.
 */

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
    void updateClientList();
    void onGeneralMessageReceived(const QString& message);

private:
    Ui::MainWindow *ui;

    ChatServer* m_chatServer;

    void setupConnections();
};

#endif // MAINWINDOW_HPP

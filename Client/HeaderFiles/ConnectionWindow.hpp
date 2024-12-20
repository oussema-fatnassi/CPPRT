#ifndef CONNECTIONWINDOW_HPP
#define CONNECTIONWINDOW_HPP

#include <QMainWindow>
#include "ChatClient.hpp"
#include "SignupWindow.hpp"
#include "MainWindow.hpp"
#include <QHostAddress>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConnectionWindow;
}
QT_END_NAMESPACE

class ConnectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectionWindow(QWidget *parent = nullptr);
    ~ConnectionWindow();

private slots:
    void onButtonConnectionClicked();
    void onSignupLinkClicked();
    void validateEmail(const QString& email);
    void validatePassword(const QString& password);

private:
    void setupConnectionWindowUI();
    bool authenticateUser(const QString& email, const QString& password, QString& username);
    bool isValidEmail(const QString& email);
    
    ChatClient* chatClient;
    Ui::ConnectionWindow* ui;
    bool isEmailValid;
    bool isPasswordValid;
    QString ipAddress;
    quint16 port;
};

#endif // CONNECTIONWINDOW_HPP
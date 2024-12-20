#ifndef CONNECTIONWINDOW_HPP
#define CONNECTIONWINDOW_HPP

#include <QMainWindow>
#include <QRegularExpression>
#include <QCryptographicHash>
#include "ChatClient.hpp"

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
    void validateEmail(const QString& email);
    void validatePassword(const QString& password);
    void onSignupLinkClicked();

private:
    void setupConnectionWindowUI();
    bool isValidEmail(const QString& email);
    bool isStrongPassword(const QString& password);
    QString hashPassword(const QString& password);
    
    ChatClient *chatClient;
    Ui::ConnectionWindow *ui;
    bool isEmailValid;
    bool isPasswordValid;
};

#endif // CONNECTIONWINDOW_HPP
#ifndef SIGNUPWINDOW_HPP
#define SIGNUPWINDOW_HPP

#include <QMainWindow>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = nullptr);
    ~SignupWindow();

private slots:
    void onCreateButtonClicked();
    void validateFields();
    void showLoginWindow(); 

private:
    Ui::SignupWindow *ui;
    bool saveUserToXML();
    bool isValidEmail(const QString& email);
};

#endif // SIGNUPWINDOW_HPP
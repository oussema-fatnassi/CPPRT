#include "SignupWindow.hpp"
#include "ui_SignupWindow.h"
#include "ConnectionWindow.hpp"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtXml>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

SignupWindow::SignupWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SignupWindow)
{
    ui->setupUi(this);
    connect(ui->createButton, &QPushButton::clicked, this, &SignupWindow::onCreateButtonClicked);
    
    // Connect the validation signals
    connect(ui->emailInput, &QLineEdit::textChanged, this, &SignupWindow::validateFields);
    connect(ui->usernameInput, &QLineEdit::textChanged, this, &SignupWindow::validateFields);
    connect(ui->passwordInput, &QLineEdit::textChanged, this, &SignupWindow::validateFields);
}

SignupWindow::~SignupWindow()
{
    delete ui;
}

bool SignupWindow::isValidEmail(const QString& email)
{
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return emailRegex.match(email).hasMatch();
}

void SignupWindow::validateFields()
{
    bool isValid = !ui->emailInput->text().isEmpty() &&
                  !ui->usernameInput->text().isEmpty() &&
                  !ui->passwordInput->text().isEmpty() &&
                  isValidEmail(ui->emailInput->text());
                  
    ui->createButton->setEnabled(isValid);
}

bool SignupWindow::saveUserToXML()
{
    // Use the current directory as the save location
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ChatApp";
    QDir dir(saveDir);
    
    // Create the directory if it doesn't exist
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QString filePath = saveDir + "/users.xml";
    QFile file(filePath);
    
    qDebug() << "Attempting to save to:" << filePath;

    // If the file doesn't exist, create it
    if (!file.exists()) {
        qDebug() << "Creating new XML file...";
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to create file:" << file.errorString();
            return false;
        }

        QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement("users");
        stream.writeEndElement();
        stream.writeEndDocument();
        file.close();
    }

    // Open the file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML content";
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.isNull()) {
        root = doc.createElement("users");
        doc.appendChild(root);
    }

    // Crate a new user
    QDomElement newUser = doc.createElement("user");
    newUser.setAttribute("firstName", ui->firstNameInput->text());
    newUser.setAttribute("lastName", ui->lastNameInput->text());
    newUser.setAttribute("username", ui->usernameInput->text());
    newUser.setAttribute("email", ui->emailInput->text());
    newUser.setAttribute("password", ui->passwordInput->text());

    root.appendChild(newUser);

    // Recreate the file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    qDebug() << "User saved successfully to" << filePath;
    return true;
}

void SignupWindow::onCreateButtonClicked()
{
    qDebug() << "Create button clicked";
    qDebug() << "First Name:" << ui->firstNameInput->text();
    qDebug() << "Last Name:" << ui->lastNameInput->text();
    qDebug() << "Username:" << ui->usernameInput->text();
    qDebug() << "Email:" << ui->emailInput->text();

    if (saveUserToXML()) {
        QMessageBox::information(this, "Success", "Account created successfully!");
        showLoginWindow();
    } else {
        QMessageBox::critical(this, "Error", 
            "Failed to create account. Please check the application logs.\n"
            "Make sure the application has write permissions in its directory.");
    }
}
void SignupWindow::showLoginWindow()
{
    ConnectionWindow* loginWindow = new ConnectionWindow(nullptr);
    loginWindow->show();
    this->close();
    this->deleteLater();
}
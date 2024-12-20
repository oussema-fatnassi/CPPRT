#include "ConnectionWindow.hpp"
#include "./ui_ConnectionWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDomDocument>
#include <QHostAddress>
#include <QRegularExpression>
#include <QCryptographicHash>
#include "ChatClient.hpp"
#include "MainWindow.hpp"
#include "SignupWindow.hpp"
#include "EnvLoader.hpp"

ConnectionWindow::ConnectionWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ConnectionWindow), chatClient(nullptr),
    isEmailValid(false), isPasswordValid(false)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Login");
    setupConnectionWindowUI();
}

ConnectionWindow::~ConnectionWindow()
{
    delete ui;
}

void ConnectionWindow::setupConnectionWindowUI()
{
    connect(ui->emailInput, &QLineEdit::textChanged, this, &ConnectionWindow::validateEmail);
    connect(ui->passwordInput, &QLineEdit::textChanged, this, &ConnectionWindow::validatePassword);
    connect(ui->connectButton, &QPushButton::clicked, this, &ConnectionWindow::onButtonConnectionClicked);
    connect(ui->signupLabel, &QLabel::linkActivated, this, &ConnectionWindow::onSignupLinkClicked);

    ui->emailValidationLabel->setStyleSheet("color: red;");
    ui->passwordValidationLabel->setStyleSheet("color: red;");

    ui->connectButton->setEnabled(false);
}

void ConnectionWindow::onSignupLinkClicked()
{
    SignupWindow *signupWindow = new SignupWindow();
    signupWindow->show();
    this->close();
}

void ConnectionWindow::validateEmail(const QString& email)
{
    if (isValidEmail(email)) {
        ui->emailValidationLabel->clear();
        isEmailValid = true;
    } else {
        ui->emailValidationLabel->setText("Invalid email format");
        isEmailValid = false;
    }
    ui->connectButton->setEnabled(isEmailValid && isPasswordValid);
}

void ConnectionWindow::validatePassword(const QString& password)
{
    // for password validation
    if (!password.isEmpty()) {
        ui->passwordValidationLabel->clear();
        isPasswordValid = true;
    } else {
        ui->passwordValidationLabel->setText("Password cannot be empty");
        isPasswordValid = false;
    }
    ui->connectButton->setEnabled(isEmailValid && isPasswordValid);
}

bool ConnectionWindow::isValidEmail(const QString& email)
{
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return emailRegex.match(email).hasMatch();
}

void ConnectionWindow::onButtonConnectionClicked()
{
    QString email = ui->emailInput->text();
    QString password = ui->passwordInput->text();

    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ChatApp";
    QString filePath = saveDir + "/users.xml";
    QFile file(filePath);

    qDebug() << "Trying to open file:" << filePath;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot access user data\nFile: " + filePath);
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::critical(this, "Error", "Invalid user data format");
        return;
    }
    file.close();

    bool authenticated = false;
    QString username;
    QDomElement root = doc.documentElement();
    QDomNodeList users = root.elementsByTagName("user");

    for (int i = 0; i < users.length(); i++) {
        QDomElement user = users.item(i).toElement();
        if (user.attribute("email") == email && user.attribute("password") == password) {
            authenticated = true;
            username = user.attribute("username");
            qDebug() << "User authenticated:" << username;
            break;
        }
    }

    if (!authenticated) {
        QMessageBox::warning(this, "Login Failed", "Invalid email or password");
        return;
    }

#ifdef _WIN32
    QFileInfo envFileInfo("../../.env");
#endif
#ifdef __APPLE__
    QFileInfo envFileInfo("../../../../../.env");
#endif

    QString envFilePath = envFileInfo.absoluteFilePath();
    if (!loadEnvFile(envFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to load server configuration");
        return;
    }

    QString ipAddress = qgetenv("CHAT_SERVER_IP");
    QString portStr = qgetenv("CHAT_SERVER_PORT");
    bool portOk;
    quint16 port = portStr.toUShort(&portOk);

    if (ipAddress.isEmpty() || !portOk) {
        QMessageBox::critical(this, "Error", "Invalid server configuration");
        return;
    }

    if (chatClient) {
        delete chatClient;
        chatClient = nullptr;
    }

    chatClient = new ChatClient(username, this);
    bool connectionResult = chatClient->connectToServer(QHostAddress(ipAddress), port);

    if (connectionResult) {
        QMessageBox::information(this, "Connection", "Successfully connected to the server!");
        MainWindow *mainWindow = new MainWindow();
        mainWindow->setChatClient(chatClient);
        mainWindow->show();
        this->close();
    } else {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server");
        delete chatClient;
        chatClient = nullptr;
    }
}
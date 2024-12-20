#include "ConnectionWindow.hpp"
#include "./ui_ConnectionWindow.h"
#include "SignupWindow.hpp"
#include <QDebug>
#include "ChatClient.hpp"
#include "EnvLoader.hpp"
#include <QMessageBox>
#include <QFileInfo>
#include "MainWindow.hpp"

ConnectionWindow::ConnectionWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ConnectionWindow), chatClient(nullptr)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Login");
    setupConnectionWindowUI();
    
    // Initialiser les drapeaux de validation
    isEmailValid = false;
    isPasswordValid = false;
}

ConnectionWindow::~ConnectionWindow()
{
    delete ui;
}

void ConnectionWindow::setupConnectionWindowUI()
{
    // Connecter les signaux pour la validation en temps réel
    connect(ui->emailInput, &QLineEdit::textChanged, this, &ConnectionWindow::validateEmail);
    connect(ui->passwordInput, &QLineEdit::textChanged, this, &ConnectionWindow::validatePassword);
    connect(ui->connectButton, &QPushButton::clicked, this, &ConnectionWindow::onButtonConnectionClicked);
     connect(ui->signupLabel, &QLabel::linkActivated, this, &ConnectionWindow::onSignupLinkClicked);

    // Style des labels de validation
    ui->emailValidationLabel->setStyleSheet("color: red;");
    ui->passwordValidationLabel->setStyleSheet("color: red;");

    // Désactiver le bouton par défaut
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
    // Activer/désactiver le bouton selon la validation
    ui->connectButton->setEnabled(isEmailValid && isPasswordValid);
}

void ConnectionWindow::validatePassword(const QString& password)
{
    if (isStrongPassword(password)) {
        ui->passwordValidationLabel->clear();
        isPasswordValid = true;
    } else {
        ui->passwordValidationLabel->setText("Password must be at least 8 characters with letters, numbers and symbols");
        isPasswordValid = false;
    }
    // Activer/désactiver le bouton selon la validation
    ui->connectButton->setEnabled(isEmailValid && isPasswordValid);
}

bool ConnectionWindow::isValidEmail(const QString& email)
{
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return emailRegex.match(email).hasMatch();
}

bool ConnectionWindow::isStrongPassword(const QString& password)
{
    // Au moins 8 caractères, avec lettres, chiffres et symboles
    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[@$!%*#?&])[A-Za-z\\d@$!%*#?&]{8,}$");
    return passwordRegex.match(password).hasMatch();
}

QString ConnectionWindow::hashPassword(const QString& password)
{
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

void ConnectionWindow::onButtonConnectionClicked()
{
    QString email = ui->emailInput->text();
    QString password = ui->passwordInput->text();

    // Vérifier si les champs sont valides
    if (!isValidEmail(email) || !isStrongPassword(password)) {
        QMessageBox::warning(this, "Invalid Input", "Please check your email and password format");
        return;
    }

    // Charger les variables d'environnement
#ifdef _WIN32
    QFileInfo envFileInfo("../../.env");
#endif
#ifdef __APPLE__
    QFileInfo envFileInfo("../../../../../.env");
#endif

    QString envFilePath = envFileInfo.absoluteFilePath();
    if (!loadEnvFile(envFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to load .env file");
        return;
    }

    // Récupérer les paramètres du serveur depuis les variables d'environnement
    QString ipAddress = qgetenv("CHAT_SERVER_IP");
    QString portStr = qgetenv("CHAT_SERVER_PORT");
    bool portOk;
    quint16 port = portStr.toUShort(&portOk);

    if (ipAddress.isEmpty() || !portOk) {
        QMessageBox::critical(this, "Connection Error", "Server IP or Port is invalid");
        return;
    }

    // Créer le client chat avec l'email comme identifiant
    QString username = email.split('@').first(); // Utiliser la partie avant @ comme nom d'utilisateur
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
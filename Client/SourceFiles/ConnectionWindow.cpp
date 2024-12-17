#include "ConnectionWindow.hpp"
#include "./ui_ConnectionWindow.h"
#include <QDebug>
#include "ChatClient.hpp"
#include "EnvLoader.hpp"
#include <QMessageBox>
#include <QFileInfo>
#include "MainWindow.hpp"

ConnectionWindow::ConnectionWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ConnectionWindow), chatClient(nullptr) {
    ui->setupUi(this);
    setFixedSize(1100, 600);
    setWindowTitle("Connection");
    setupConnectionWindowUI();
}


ConnectionWindow:: ~ConnectionWindow()
{
    delete ui;
}

void ConnectionWindow:: setupConnectionWindowUI()
{
    connect(ui->connectButton, &QPushButton::clicked, this, &ConnectionWindow::onButtonConnectionClicked);
}

void ConnectionWindow::onButtonConnectionClicked() {
    // Fetch the text from the username input field
    QString username = ui->usernameInput->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Username cannot be empty");
        return;
    }

// Load environment variables
#ifdef _WIN32
    QFileInfo envFileInfo("../../.env");
#endif
#ifdef __APPLE__
    QFileInfo envFileInfo("../../../../../.env");
#endif

    QString envFilePath = envFileInfo.absoluteFilePath();
    qDebug() << "Env File Path: " << envFilePath;

    if (!loadEnvFile(envFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to load .env file");
        return;
    }

    QString ipAddress = qgetenv("CHAT_SERVER_IP");
    QString portStr = qgetenv("CHAT_SERVER_PORT");
    qDebug() << "IP Address:" << ipAddress << ", Port:" << portStr;

    bool portOk;
    quint16 port = portStr.toUShort(&portOk);

    if (ipAddress.isEmpty() || !portOk) {
        QMessageBox::critical(this, "Connection Error", "Server IP or Port is invalid");
        return;
    }

    // Safely delete the old ChatClient if it exists
    if (chatClient) {
        delete chatClient;
        chatClient = nullptr;
    }

    // Create a new ChatClient
    chatClient = new ChatClient(username, this);
    bool connectionResult = chatClient->connectToServer(QHostAddress(ipAddress), port);

    if (connectionResult) {
        QMessageBox::information(this, "Connection", "Successfully connected to the server!");
        MainWindow *mainWindow = new MainWindow();
        mainWindow->setChatClient(chatClient);

        // Show the MainWindow and close ConnectionWindow
        mainWindow->show();
        this->close();
    } else {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server");
        delete chatClient;
        chatClient = nullptr;
    }
}

#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(ChatServer* server, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_chatServer(server)
{
    ui->setupUi(this);
    setWindowTitle("Server");

    // Setup widget connections
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // Connect the disconnect button
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);
    connect(ui->disconnectAllButton, &QPushButton::clicked, this, &MainWindow::onDisconnectAllButtonClicked);

    // Connect the send buttons in the tabs
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSendMessageTab1Clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onSendMessageTab2Clicked);
}

void MainWindow::onDisconnectButtonClicked()
{
    // Retrieve the selected item in the client list
    QListWidgetItem *selectedItem = ui->listWidget->currentItem();
    if (selectedItem) {
        // Extract the client identifier (e.g., the username)
        QString clientIdentifier = selectedItem->text();

        // Disconnect the client via the server
        if (m_chatServer) {
            QTcpSocket* clientSocket = m_chatServer->getSocketByName(clientIdentifier);
            if (clientSocket) {
                clientSocket->disconnectFromHost(); // Triggers handleDisconnection in ChatServer
                qDebug() << "Client" << clientIdentifier << "disconnected successfully.";
            } else {
                qDebug() << "Client with identifier" << clientIdentifier << "not found on the server.";
            }
        }

        // Remove the item from the client list
        delete selectedItem;
    }
}

void MainWindow::onDisconnectAllButtonClicked()
{
    // Iterate through all items in the client list and disconnect each client
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item) {
            QString clientIdentifier = item->text();
            if (m_chatServer) {
                QTcpSocket* clientSocket = m_chatServer->getSocketByName(clientIdentifier);
                if (clientSocket) {
                    clientSocket->disconnectFromHost(); // Triggers handleDisconnection in ChatServer
                    qDebug() << "Client" << clientIdentifier << "disconnected successfully.";
                } else {
                    qDebug() << "Client with identifier" << clientIdentifier << "not found on the server.";
                }
            }
        }
    }

    // Clear the client list
    ui->listWidget->clear();
}

void MainWindow::onSendMessageTab1Clicked()
{
    // Placeholder to send a message in Tab 1
    qDebug() << "Send message button clicked in Tab 1.";
}

void MainWindow::onSendMessageTab2Clicked()
{
    // Placeholder to send a message in Tab 2
    qDebug() << "Send message button clicked in Tab 2.";
}

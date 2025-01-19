#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(ChatServer* server, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_chatServer(server)
{
    ui->setupUi(this);
    setWindowTitle("Server");

    connect(m_chatServer, &ChatServer::generalMessageReceived, this, &MainWindow::onGeneralMessageReceived);

    // Setup widget connections
    setupConnections();
    connect(m_chatServer, &ChatServer::clientListChanged, this, &MainWindow::updateClientList);
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
}

void MainWindow::onDisconnectButtonClicked() {
    // Retrieve the selected item in the client list
    QListWidgetItem *selectedItem = ui->listWidget->currentItem();
    if (selectedItem) {
        QString clientIdentifier = selectedItem->text();

        // Request the server to disconnect the client
        if (m_chatServer) {
            // Use the server's existing disconnection mechanism
            m_chatServer->disconnectClientByName(clientIdentifier);
        }
    }
}

void MainWindow::onDisconnectAllButtonClicked()
{
    // Create a copy of the client list to avoid modifying the list while iterating
    QList<QString> clientIdentifiers;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item) {
            clientIdentifiers.append(item->text());
        }
    }

    // Disconnect each client
    for (const QString& clientIdentifier : clientIdentifiers) {
        if (m_chatServer) {
            m_chatServer->disconnectClientByName(clientIdentifier);
        }
    }
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

void MainWindow::updateClientList() {
    if (m_chatServer) {
        QList<QString> clientList = m_chatServer->getConnectedClients();

        // Clear and repopulate the client list
        ui->listWidget->clear();
        for (const QString &client : clientList) {
            ui->listWidget->addItem(client);
        }
    }
}

void MainWindow::onGeneralMessageReceived(const QString& message) {
    if (ui->listWidgetGeneralMessages) { // Ensure the widget exists
        QString cleanedMessage = message;

        // Remove the "BROADCAST:" prefix if it exists
        if (cleanedMessage.startsWith("BROADCAST:")) {
            cleanedMessage = cleanedMessage.mid(QString("BROADCAST:").length());
        }

        ui->listWidgetGeneralMessages->addItem(cleanedMessage); // Add the cleaned message
    } else {
        qDebug() << "listWidgetGeneralMessages widget not found in the UI.";
    }
}




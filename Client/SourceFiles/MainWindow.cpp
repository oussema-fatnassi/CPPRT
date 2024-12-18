#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "WizzEffect.hpp"
#include <QStandardItemModel>
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), chatClient(nullptr)
{
    ui->setupUi(this);
    setFixedSize(1100, 600);
    setWindowTitle("Client");

    // Initialize the model for the QListView
    listViewModel = new QStandardItemModel(this);
    ui->listView->setModel(listViewModel);

    // Set up connections
    setupMainWindowUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setChatClient(ChatClient* client) {
    this->chatClient = client;
    qDebug() << "ChatClient passed to MainWindow!";

    // Connect signals for message reception, user list updates and disconnection
    connect(chatClient, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(chatClient, &ChatClient::userListUpdated, this, &MainWindow::updateUserList);
    connect(chatClient, &ChatClient::disconnected, this, &MainWindow::handleServerDisconnection);
}

void MainWindow::setupMainWindowUI()
{
    // Connect buttons to their respective slots
    connect(ui->buttonSend, &QPushButton::clicked, this, &MainWindow::onBtnSendClicked);
    connect(ui->buttonWizz, &QPushButton::clicked, this, &MainWindow::onPushButtonWizzClicked);

    ui->textEdit->installEventFilter(this);
}

void MainWindow::updateUserList(const QStringList& users) {
    // Clear the list widget and repopulate with filtered users
    ui->listClients->clear();
    for (const QString& user : users) {
        ui->listClients->addItem(user);
    }
}

void MainWindow::onBtnSendClicked()
{
    // Get the text from the input field
    QString message = ui->textEdit->toPlainText();

    // Do not proceed if the message is empty
    if (message.isEmpty()) {
        return;
    }

    // Add the message to the list view with a timestamp
    QString messageWithTimestamp = QString("[%1] %2")
                                       .arg(QTime::currentTime().toString("hh:mm:ss"))
                                       .arg(message);

    if (chatClient) {
        chatClient->sendMessage(message);
    }

    QStandardItem *item = new QStandardItem(messageWithTimestamp);
    listViewModel->appendRow(item);

    // Clear the input field
    ui->textEdit->clear();

    // Scroll to the bottom of the list view
    ui->listView->scrollToBottom();
    ui->listView->setWordWrap(true);
}

void MainWindow::onPushButtonWizzClicked()
{
    // Trigger the WizzEffect on the main window
    WizzEffect *wizzEffect = new WizzEffect(this);
    wizzEffect->start();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // Check if Shift+Enter is pressed
        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) &&
            keyEvent->modifiers() & Qt::ShiftModifier) {
            // Insert a new line
            QTextCursor cursor = ui->textEdit->textCursor();
            cursor.insertText("\n");
            return true;
        }

        // Check if only Enter is pressed
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            onBtnSendClicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onMessageReceived(const QString& message) {
    // Format the message with a timestamp
    QString messageWithTimestamp = QString("[%1] %2")
                                       .arg(QTime::currentTime().toString("hh:mm:ss"))
                                       .arg(message);

    QStandardItem* item = new QStandardItem(messageWithTimestamp);
    listViewModel->appendRow(item);

    // Scroll to the bottom
    ui->listView->scrollToBottom();
}

void MainWindow::handleServerDisconnection()
{
    // Create and configure the disconnection message box
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Disconnection");
    msgBox->setText("You have been disconnected.");
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setStandardButtons(QMessageBox::Ok);

    // Connect the OK button to close the window
    connect(msgBox, &QMessageBox::finished, this, [this]() {
        // Cleanup before closing
        if (chatClient) {
            chatClient->deleteLater();
            chatClient = nullptr;
        }
        // Close the main window
        this->close();
    });

    // Show the message box
    msgBox->show();
    
    // Disable UI controls
    ui->buttonSend->setEnabled(false);
    ui->buttonWizz->setEnabled(false);
    ui->textEdit->setEnabled(false);
}
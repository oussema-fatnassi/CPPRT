#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "WizzEffect.hpp"
#include <QStandardItemModel>
#include <QTime>

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

    // Connect signals BEFORE potentially triggering any updates
    connect(chatClient, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(chatClient, &ChatClient::userListUpdated, this, &MainWindow::updateUserList);

    // Optional: Trigger an initial update if needed
    if (!chatClient->getOnlineUsers().isEmpty()) {
        updateUserList(chatClient->getOnlineUsers());
    }
}

void MainWindow::setupMainWindowUI()
{
    // Connect buttons to their respective slots
    connect(ui->buttonSend, &QPushButton::clicked, this, &MainWindow::onBtnSendClicked);
    connect(ui->buttonWizz, &QPushButton::clicked, this, &MainWindow::onPushButtonWizzClicked);
    connect(ui->listClients, &QListWidget::itemDoubleClicked, this, &MainWindow::onUserDoubleClicked);

    ui->textEdit->installEventFilter(this);
}

void MainWindow::onUserDoubleClicked(QListWidgetItem* item) {
    // Get the user's name from the clicked item
    QString username = item->text();

    // Check if a tab with this username already exists
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (ui->tabWidget->tabText(i) == username) {
            ui->tabWidget->setCurrentIndex(i); // Focus the existing tab
            return;
        }
    }

    // Create a new tab
    QWidget* newTab = new QWidget(this);

    // Add a QListView to the tab
    QListView* listView = new QListView(newTab);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setWrapping(false);
    listView->setWordWrap(true);

    // Set the same size policy as the General Channel's QListView
    listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Set the layout for the new tab
    QVBoxLayout* layout = new QVBoxLayout(newTab);
    layout->setContentsMargins(0, 0, 0, 0); // Match the margins in the UI file
    layout->addWidget(listView);
    newTab->setLayout(layout);

    // Add the tab to the tab widget with the user's name as the title
    ui->tabWidget->addTab(newTab, username);
    ui->tabWidget->setCurrentWidget(newTab);
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

    // Get the current tab's name
    QString currentTab = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    if(currentTab == "General Channel") {
        // Add the message to the list view with a timestamp
        QString messageWithTimestamp = QString("[%1] %2")
                                           .arg(QTime::currentTime().toString("hh:mm:ss"))
                                           .arg(message);

        if (chatClient) {
            chatClient->sendMessage(message);
        }

        QStandardItem *item = new QStandardItem(messageWithTimestamp);
        listViewModel->appendRow(item);
    }

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
    ui->listView->setWordWrap(true);
}

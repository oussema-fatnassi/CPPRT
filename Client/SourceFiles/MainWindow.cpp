#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include <QDebug>
#include "MessageBubbleManager.hpp"

    MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chatClient(nullptr)
{
    ui->setupUi(this);
    setFixedSize(1100, 600);
    setWindowTitle("Client");

    // Initialize models
    listViewModel = new QStandardItemModel(this);
    ui->listView->setModel(listViewModel);

    // Initialize handlers and managers
    messageHandler = new MessageHandler(this);
    messageHandler->setGeneralChannelModel(listViewModel);

    tabManager = new TabManager(ui->tabWidget, messageHandler, this);

    uiEventHandler = new UIEventHandler(
        ui->textEdit,
        ui->buttonSend,
        ui->listClients,
        chatClient,
        tabManager,
        messageHandler,
        this
        );

    wizzManager = new WizzManager(
        ui->listClients,
        chatClient,
        this,
        this
        );

    setupMainWindowUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setChatClient(ChatClient* client)
{
    this->chatClient = client;
    qDebug() << "ChatClient passed to MainWindow!";

    // Update chat client reference in handlers
    uiEventHandler->setChatClient(client);
    wizzManager->setChatClient(client);

    // Connect signals
    connect(chatClient, &ChatClient::messageReceived, this, &MainWindow::onGeneralMessageReceived);
    connect(chatClient, &ChatClient::privateMessageReceived, this, &MainWindow::onPrivateMessageReceived);
    connect(chatClient, &ChatClient::userListUpdated, uiEventHandler, &UIEventHandler::updateUserList);
    connect(chatClient, &ChatClient::wizzReceived, wizzManager, &WizzManager::triggerWizzEffect);
    connect(chatClient, &ChatClient::disconnected, this, &MainWindow::handleServerDisconnection);

    // Initialize user list if available
    if (!chatClient->getOnlineUsers().isEmpty()) {
        uiEventHandler->updateUserList(chatClient->getOnlineUsers());
    }
}

void MainWindow::setupMainWindowUI()
{
    // Set up main window UI connections
    connect(ui->buttonSend, &QPushButton::clicked,
            uiEventHandler, &UIEventHandler::onSendClicked);
    connect(ui->buttonWizz, &QPushButton::clicked,
            wizzManager, &WizzManager::onWizzButtonClicked);
    connect(ui->listClients, &QListWidget::itemDoubleClicked,
            uiEventHandler, &UIEventHandler::onUserDoubleClicked);

    // Install event filter for text edit
    ui->textEdit->installEventFilter(this);

    // Set up list view properties
    ui->listView->setWordWrap(true);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set up the MessageBubbleManager for the general channel
    MessageBubbleManager* delegate = new MessageBubbleManager(ui->listView);
    ui->listView->setItemDelegate(delegate);

    // Configure the list view for better bubble display
    ui->listView->setSpacing(8);
    ui->listView->setUniformItemSizes(false);
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set initial focus to text edit
    ui->textEdit->setFocus();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    // Delegate event handling to UIEventHandler
    if (uiEventHandler->handleKeyPress(obj, event)) {
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onGeneralMessageReceived(const QString& message)
{
    messageHandler->addMessageToGeneral(message, false);
    ui->listView->scrollToBottom();
}

void MainWindow::onPrivateMessageReceived(const QString& senderName,
                                          const QString& recipient,
                                          const QString& dmContent)
{
    // Determine which tab to use (sender or recipient)
    QString tabUsername = (senderName == chatClient->getName()) ? recipient : senderName;

    // Get or create the appropriate tab and its list view
    QListView* listView = tabManager->getOrCreateTab(tabUsername);
    if (!listView) {
        qDebug() << "Failed to get or create tab for private message";
        return;
    }

    // If this is a local message (we sent it)
    if (senderName == chatClient->getName()) {
        // Check for duplicate messages
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(listView->model());
        if (model) {
            for (int i = 0; i < model->rowCount(); ++i) {
                QStandardItem* existingItem = model->item(i);
                if (existingItem->data(Qt::UserRole).toBool() &&
                    existingItem->text() == dmContent) {  // Direct content comparison
                    return;  // Skip duplicate messages
                }
            }
        }
    }

    // Just add the content without any sender prefix
    messageHandler->addMessageToTab(listView, dmContent,
                                    senderName == chatClient->getName());
    listView->scrollToBottom();

    // Update UI handlers since we've modified the client
    if (!uiEventHandler->hasChatClient()) {
        uiEventHandler->setChatClient(chatClient);
    }
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

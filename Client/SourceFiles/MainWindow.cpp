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
    // disconnect(chatClient, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(chatClient, &ChatClient::messageReceived, this, &MainWindow::onGeneralMessageReceived);

    // Connect the new private message signal
    connect(chatClient, &ChatClient::privateMessageReceived,
            this, &MainWindow::onPrivateMessageReceived);

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

    // Get the current tab's name (recipient name for DMs)
    QString currentTab = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    if (currentTab == "General Channel") {
        // Handle general broadcast messages
        QString messageWithTimestamp = QString("[%1] %2")
                                           .arg(QTime::currentTime().toString("hh:mm:ss"))
                                           .arg(message);

        if (chatClient) {
            chatClient->sendMessage(message);
        }

        QStandardItem *item = new QStandardItem(messageWithTimestamp);
        listViewModel->appendRow(item);
    } else {
        // Handle direct messages
        QString messageWithTimestamp = QString("[%1] You: %2")
                                           .arg(QTime::currentTime().toString("hh:mm:ss"))
                                           .arg(message);

        if (chatClient) {
            chatClient->sendDirectMessage(currentTab, message);
        }

        // Add the message to the current tab
        QWidget *currentWidget = ui->tabWidget->currentWidget();
        QListView *listView = currentWidget->findChild<QListView *>();
        if (listView) {
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(listView->model());
            if (!model) {
                model = new QStandardItemModel(listView);
                listView->setModel(model);
            }
            QStandardItem *item = new QStandardItem(messageWithTimestamp);
            model->appendRow(item);
        }
    }

    // Clear the input field
    ui->textEdit->clear();

    // Scroll to the bottom of the active list view
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    QListView *listView = currentWidget->findChild<QListView *>();
    if (listView) {
        listView->scrollToBottom();
    }
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

// void MainWindow::onMessageReceived(const QString& message) {

//     if (message.startsWith("DM:")) {
//         // Extract sender's name and message content
//         QString senderName = message.section(":", 1, 1);
//         QString dmContent = message.section(":", 2);

//         // Find or create a tab for the sender
//         QWidget* tab = nullptr;
//         QListView* listView = nullptr;

//         // Search for existing tab
//         for (int i = 0; i < ui->tabWidget->count(); ++i) {
//             if (ui->tabWidget->tabText(i) == senderName) {
//                 tab = ui->tabWidget->widget(i);
//                 break;
//             }
//         }

//         // If the tab does not exist, create it automatically
//         if (!tab) {
//             // Simulate a user double-click to create the tab
//             QListWidgetItem* newItem = new QListWidgetItem(senderName);
//             onUserDoubleClicked(newItem);
//             delete newItem;

//             // Find the newly created tab
//             for (int i = 0; i < ui->tabWidget->count(); ++i) {
//                 if (ui->tabWidget->tabText(i) == senderName) {
//                     tab = ui->tabWidget->widget(i);
//                     break;
//                 }
//             }
//         }

//         // Ensure we have the correct tab and list view
//         if (tab) {
//             listView = tab->findChild<QListView *>();
//             if (listView) {
//                 QStandardItemModel* model = qobject_cast<QStandardItemModel*>(listView->model());
//                 if (!model) {
//                     model = new QStandardItemModel(listView);
//                     listView->setModel(model);
//                 }

//                 // Format the message and add it to the list view
//                 QString timestampedMessage = QString("[%1] %2: %3")
//                                                  .arg(QTime::currentTime().toString("hh:mm:ss"))
//                                                  .arg(senderName)
//                                                  .arg(dmContent);
//                 QStandardItem* item = new QStandardItem(timestampedMessage);
//                 model->appendRow(item);

//                 // Scroll to the bottom
//                 listView->scrollToBottom();
//             }
//         }
//     } else {
//         // Format the message with a timestamp
//         QString messageWithTimestamp = QString("[%1] %2")
//                                            .arg(QTime::currentTime().toString("hh:mm:ss"))
//                                            .arg(message);

//         QStandardItem* item = new QStandardItem(messageWithTimestamp);
//         listViewModel->appendRow(item);
//     }


//     // Scroll to the bottom
//     ui->listView->scrollToBottom();
//     ui->listView->setWordWrap(true);
// }

void MainWindow::onGeneralMessageReceived(const QString& message) {
    // Format the message with a timestamp for general channel
    QString messageWithTimestamp = QString("[%1] %2")
                                       .arg(QTime::currentTime().toString("hh:mm:ss"))
                                       .arg(message);

    QStandardItem* item = new QStandardItem(messageWithTimestamp);
    listViewModel->appendRow(item);

    // Scroll to the bottom
    ui->listView->scrollToBottom();
    ui->listView->setWordWrap(true);
}

// New method to handle private messages
void MainWindow::onPrivateMessageReceived(const QString &senderName, const QString &recipient, const QString &dmContent) {
    // Determine the correct tab for this message
    QString tabUsername = (senderName == chatClient->getName()) ? recipient : senderName;

    // Check if the tab for this user already exists
    QWidget *tab = nullptr;
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (ui->tabWidget->tabText(i) == tabUsername) {
            tab = ui->tabWidget->widget(i);
            break;
        }
    }

    // If no tab exists, create one
    if (!tab) {
        QListWidgetItem *newItem = new QListWidgetItem(tabUsername);
        onUserDoubleClicked(newItem);
        delete newItem;

        // Find the newly created tab
        for (int i = 0; i < ui->tabWidget->count(); ++i) {
            if (ui->tabWidget->tabText(i) == tabUsername) {
                tab = ui->tabWidget->widget(i);
                break;
            }
        }
    }

    // Add the message to the tab's QListView
    if (tab) {
        QListView *listView = tab->findChild<QListView *>();
        if (listView) {
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(listView->model());
            if (!model) {
                model = new QStandardItemModel(listView);
                listView->setModel(model);
            }

            // Format the message
            QString timestampedMessage;
            if (senderName == chatClient->getName()) {
                timestampedMessage = QString("[%1] You: %2")
                .arg(QTime::currentTime().toString("hh:mm:ss"))
                    .arg(dmContent);
            } else {
                timestampedMessage = QString("[%1] %2: %3")
                .arg(QTime::currentTime().toString("hh:mm:ss"))
                    .arg(senderName)
                    .arg(dmContent);
            }

            QStandardItem *item = new QStandardItem(timestampedMessage);
            model->appendRow(item);

            // Scroll to the bottom
            listView->scrollToBottom();
        }
    }
}



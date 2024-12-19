#include "MainWindow.hpp"
#include "WizzEffect.hpp"
#include "UIManager.hpp"
#include <QStandardItemModel>
#include <QTime>
#include <QMessageBox>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), chatClient(nullptr)
{
    setupUi();
    setupConnections();
    setFixedSize(1100, 600);
    setWindowTitle("Client");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    // Create central widget and main layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Create top section with chat and users list
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    // Chat area
    textBrowser = new QTextBrowser();
    UIManager::setupTextBrowser(textBrowser);
    topLayout->addWidget(textBrowser, 7); // 70% width

    // Update textBrowser style
    textBrowser->setStyleSheet(
        "QTextBrowser {"
        "   background-color: #F5F5F5;"  // Light gray background
        "   border: none;"
        "   padding: 10px;"
        "}"
    );
    textBrowser->setOpenExternalLinks(false);
    textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textBrowser->document()->setDefaultStyleSheet(
        "body { margin: 0; padding: 0; }"
        ".message-sent { background-color: #0078D4; color: white; width: 100%; }"
        ".message-received { background-color: white; border: 1px solid #E0E0E0; color: black; width: 100%; }"
        ".timestamp { color: #666666; font-size: 12px; }"
        ".sender { font-weight: bold; color: #333333; }"
    );
    
    // Users list
    listClients = new QListWidget();
    UIManager::setupUserList(listClients);
    topLayout->addWidget(listClients, 3); // 30% width
    
    mainLayout->addLayout(topLayout);
    
    // Create bottom section for input
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    
    // Message input
    textEdit = new QTextEdit();
    textEdit->setMaximumHeight(50);
    UIManager::setupTextEdit(textEdit);
    bottomLayout->addWidget(textEdit);
    
    // Emoji button
    setupEmojiButton();
    bottomLayout->addWidget(emojiButton);
    
    // Buttons
    buttonSend = new QPushButton("Send");
    buttonWizz = new QPushButton("Wizz");
    UIManager::setupButtons(buttonSend, buttonWizz);
    bottomLayout->addWidget(buttonSend);
    bottomLayout->addWidget(buttonWizz);
    
    mainLayout->addLayout(bottomLayout);
    
    setCentralWidget(centralWidget);
    textEdit->installEventFilter(this);
}
void MainWindow::setupEmojiButton()
{
    emojiButton = new QPushButton("😊", this);
    emojiButton->setFixedSize(30, 30);
    emojiButton->setStyleSheet("QPushButton { border: none; }");

    emojiMenu = new QMenu(this);

    QMap<QString, QStringList> emojiCategories = {
        {"Smileys", {"😊", "😄", "😆", "😅", "😂", "🤣", "😊", "😇", "🙂", "😉", "😍"}},
        {"Emotions", {"❤️", "😘", "🥰", "😊", "😌", "🤗", "😔", "😢", "😭", "😤", "😠"}},
        {"Gestures", {"👍", "👎", "👋", "✌️", "🤞", "👊", "🤝", "👏", "🙌", "🤲", "🤦"}},
        {"Symbols", {"⭐", "🔥", "💫", "💥", "💢", "💦", "💨", "🕊️", "🎵", "🎶", "💭"}}
    };

    for (auto it = emojiCategories.begin(); it != emojiCategories.end(); ++it) {
        QMenu* categoryMenu = new QMenu(it.key(), emojiMenu);
        emojiMenu->addMenu(categoryMenu);

        for (const QString& emoji : it.value()) {
            QAction* action = new QAction(emoji, this);
            connect(action, &QAction::triggered, this, [this, emoji]() {
                addEmojiToMessage(emoji);
            });
            categoryMenu->addAction(action);
        }
    }

    connect(emojiButton, &QPushButton::clicked, this, [this]() {
        emojiMenu->popup(emojiButton->mapToGlobal(QPoint(0, -emojiMenu->sizeHint().height())));
    });
}

void MainWindow::addEmojiToMessage(const QString& emoji)
{
    textEdit->insertPlainText(emoji);
}

void MainWindow::setupConnections()
{
    connect(buttonSend, &QPushButton::clicked, this, &MainWindow::onBtnSendClicked);
    connect(buttonWizz, &QPushButton::clicked, this, &MainWindow::onPushButtonWizzClicked);
}

void MainWindow::setChatClient(ChatClient* client) {
    this->chatClient = client;
    qDebug() << "ChatClient passed to MainWindow!";

    connect(chatClient, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(chatClient, &ChatClient::userListUpdated, this, &MainWindow::updateUserList);
    connect(chatClient, &ChatClient::disconnected, this, &MainWindow::handleServerDisconnection);
    connect(chatClient, &ChatClient::wizzReceived, this, &MainWindow::onWizzReceived);
}
void MainWindow::onWizzReceived(const QString& sender)
{
    qDebug() << "Wizz received from:" << sender;  // Pour le debug
    if (sender != chatClient->getName()) {
        WizzEffect* wizzEffect = new WizzEffect(this);
        wizzEffect->start();
    }
}
void MainWindow::updateUserList(const QStringList& users) {
    listClients->clear();
    for (const QString& user : users) {
        listClients->addItem(user);
    }
}

void MainWindow::onPushButtonWizzClicked()
{
    // Récupérer le client sélectionné dans la liste
    QListWidgetItem* selectedItem = listClients->currentItem();
    if (!selectedItem) {
        return;  // Aucun client sélectionné
    }

    QString targetName = selectedItem->text();
    if (chatClient) {
        chatClient->sendWizz(targetName);
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) &&
            keyEvent->modifiers() & Qt::ShiftModifier) {
            QTextCursor cursor = textEdit->textCursor();
            cursor.insertText("\n");
            return true;
        }

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            onBtnSendClicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onMessageReceived(const QString& message) {
    textBrowser->append(UIManager::formatMessage(message, false));
    textBrowser->verticalScrollBar()->setValue(
        textBrowser->verticalScrollBar()->maximum()
    );
}

void MainWindow::onBtnSendClicked()
{
    QString message = textEdit->toPlainText();
    if (message.isEmpty()) {
        return;
    }

    if (chatClient) {
        chatClient->sendMessage(message);
    }

    textBrowser->append(UIManager::formatMessage(message, true));
    textEdit->clear();
    textBrowser->verticalScrollBar()->setValue(
        textBrowser->verticalScrollBar()->maximum()
    );
}
void MainWindow::handleServerDisconnection()
{
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Disconnection");
    msgBox->setText("You have been disconnected.");
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setStandardButtons(QMessageBox::Ok);

    connect(msgBox, &QMessageBox::finished, this, [this]() {
        if (chatClient) {
            chatClient->deleteLater();
            chatClient = nullptr;
        }
        this->close();
    });

    msgBox->show();
    
    buttonSend->setEnabled(false);
    buttonWizz->setEnabled(false);
    textEdit->setEnabled(false);
}
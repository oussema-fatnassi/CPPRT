#include "MainWindow.hpp"
#include "WizzEffect.hpp"
#include <QStandardItemModel>
#include <QTime>
#include <QMessageBox>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
    textBrowser->setOpenExternalLinks(false);
    textBrowser->setTextInteractionFlags(Qt::TextBrowserInteraction);
    topLayout->addWidget(textBrowser, 7); // 70% width
    
    // Users list
    listClients = new QListWidget();
    topLayout->addWidget(listClients, 3); // 30% width
    
    mainLayout->addLayout(topLayout);
    
    // Create bottom section for input
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    
    // Message input
    textEdit = new QTextEdit();
    textEdit->setMaximumHeight(50);
    textEdit->installEventFilter(this);
    bottomLayout->addWidget(textEdit);
    
    // Emoji button
    setupEmojiButton();
    bottomLayout->addWidget(emojiButton);
    
    // Buttons
    buttonSend = new QPushButton("Send");
    buttonWizz = new QPushButton("Wizz");
    bottomLayout->addWidget(buttonSend);
    bottomLayout->addWidget(buttonWizz);
    
    mainLayout->addLayout(bottomLayout);
    
    setCentralWidget(centralWidget);

textBrowser->setStyleSheet(
        "QTextBrowser {"
        "   background-color: white;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   border: 1px solid rgb(10, 9, 9);"
        "}"
    );

    // Style moderne pour la zone de saisie
    textEdit->setStyleSheet(
        "QTextEdit {"
        "   border: 1px solid rgb(219, 203, 203);"
        "   border-radius: 20px;"
        "   padding: 10px;"
        "   background-color: white;"
        "   margin: 5px;"
        "}"
    );

    // Style pour les boutons
    QString buttonStyle = 
        "QPushButton {"
        "   background-color: #0084ff;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding: 8px 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0073e6;"
        "}";

    buttonSend->setStyleSheet(buttonStyle);
    buttonWizz->setStyleSheet(buttonStyle);

    // Style pour la liste des utilisateurs
    listClients->setStyleSheet(
        "QListWidget {"
        "   background-color: white;"
        "   border: 1px solid rgb(10, 9, 9);"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item {"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color:rgb(88, 120, 155);"
        "   color: #0084ff;"
        "}"
    );

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

QString MainWindow::replaceEmojis(QString message) {
    QMap<QString, QString> emojiMap = {
        {":)", "😊"},
        {":-)", "😊"},
        {":(", "😢"},
        {":-(", "😢"},
        {":D", "😄"},
        {":-D", "😄"},
        {"<3", "❤️"},
        {":p", "😛"},
        {":P", "😛"},
        {";)", "😉"},
        {":o", "😮"},
        {":O", "😮"},
        {"xD", "😆"},
        {"XD", "😆"},
        {":*", "😘"},
        {":|", "😐"},
        {":-|", "😐"},
        {">:(", "😠"},
        {":@", "😠"},
        {":'(", "😢"}
    };

    for (auto it = emojiMap.begin(); it != emojiMap.end(); ++it) {
        message.replace(it.key(), it.value());
    }
    return message;
}

void MainWindow::setupConnections()
{
    connect(buttonSend, &QPushButton::clicked, this, &MainWindow::onBtnSendClicked);
    connect(buttonWizz, &QPushButton::clicked, this, &MainWindow::onPushButtonWizzClicked);
}

void MainWindow::setChatClient(ChatClient* client) {
    this->chatClient = client;
    qDebug() << "ChatClient passed to MainWindow!";

    connect(chatClient, &ChatClient::messageReceived, 
            this, &MainWindow::onMessageReceived);
    connect(chatClient, &ChatClient::userListUpdated, 
            this, &MainWindow::updateUserList);
    connect(chatClient, &ChatClient::disconnected, 
            this, &MainWindow::handleServerDisconnection);

    if (!chatClient->getOnlineUsers().isEmpty()) {
        updateUserList(chatClient->getOnlineUsers());
    }
}
void MainWindow::updateUserList(const QStringList& users) {
    listClients->clear();
    for (const QString& user : users) {
        listClients->addItem(user);
    }
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

    textBrowser->append(formatMessage(message, true));
    textEdit->clear();
    textBrowser->verticalScrollBar()->setValue(
        textBrowser->verticalScrollBar()->maximum()
    );
}

QString MainWindow::formatMessage(const QString& message, bool isSentByMe) {
    QString timeStamp = QTime::currentTime().toString("hh:mm");
    QString formattedMessage = replaceEmojis(message);
    
    // Style moderne avec des bulles de message arrondies
    QString style = QString(
        "<div style='margin: 10px 20px; %1'>"
        "<div style='display: inline-block; max-width: 70%%; %2'>"
        "<div style='%3 padding: 12px; border-radius: 18px; position: relative;'>"
        "<span style='font-size: 14px; color: #%4'>%5</span>"
        "<div style='font-size: 11px; color: #999; margin-top: 5px; text-align: right'>%6</div>"
        "</div>"
        "</div>"
        "</div>"
    );

    if (isSentByMe) {
        return style
            .arg("text-align: right")                            // Alignement à droite
            .arg("margin-left: 30%")                            // Marge pour les messages envoyés
            .arg("background-color: #0084ff; color: white")     // Bleu pour mes messages
            .arg("000000")                                      // Texte blanc
            .arg(formattedMessage)
            .arg(timeStamp);
    } else {
        return style
            .arg("text-align: left")                           // Alignement à gauche
            .arg("margin-right: 30%")                          // Marge pour les messages reçus
            .arg("background-color:rgb(95, 74, 74); color: black")    // Gris clair pour les autres
            .arg("000000")                                     // Texte noir
            .arg(formattedMessage)
            .arg(timeStamp);
    }
}
void MainWindow::onPushButtonWizzClicked()
{
    WizzEffect *wizzEffect = new WizzEffect(this);
    wizzEffect->start();
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
    textBrowser->append(formatMessage(message, false));
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
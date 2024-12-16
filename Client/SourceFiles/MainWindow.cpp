#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "WizzEffect.hpp"
#include <QStandardItemModel>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Client");

    // Initialize the model for the QListView
    listViewModel = new QStandardItemModel(this);
    ui->listView->setModel(listViewModel);

    // Set up connections
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // Connect buttons to their respective slots
    connect(ui->buttonSend, &QPushButton::clicked, this, &MainWindow::onBtnSendClicked);
    connect(ui->buttonWizz, &QPushButton::clicked, this, &MainWindow::onPushButtonWizzClicked);

    ui->textEdit->installEventFilter(this);


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
            return true; // Consume the event
        }

        // Check if only Enter is pressed
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            onBtnSendClicked(); // Trigger send action
            return true;        // Consume the event
        }
    }
    return QMainWindow::eventFilter(obj, event);
}



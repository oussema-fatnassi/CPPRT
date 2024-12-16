#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Server");

    // Set up connections
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // Connect the Disconnect button
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);

    // Connect the Send buttons in the tabs
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSendMessageTab1Clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onSendMessageTab2Clicked);

    // Connect the checkbox state change
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxStateChanged);
}

void MainWindow::onDisconnectButtonClicked()
{
    // Placeholder for disconnect functionality
}

void MainWindow::onSendMessageTab1Clicked()
{
    // Placeholder for sending a message in Tab 1
}

void MainWindow::onSendMessageTab2Clicked()
{
    // Placeholder for sending a message in Tab 2
}

void MainWindow::onCheckBoxStateChanged(int state)
{
    // Placeholder for handling checkbox state changes
}

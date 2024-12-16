#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "WizzEffect.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Client");

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
    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onBtnSendClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onPushButtonWizzClicked);
}

void MainWindow::onBtnSendClicked()
{
    // Functionality for the Send button
}

void MainWindow::onPushButtonWizzClicked()
{
    WizzEffect *wizzEffect = new WizzEffect(this);  // Corrected line
    wizzEffect->start();

}

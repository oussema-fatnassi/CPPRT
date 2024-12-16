#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFrame>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDisconnectButtonClicked();
    void onSendMessageTab1Clicked();
    void onSendMessageTab2Clicked();
    void onCheckBoxStateChanged(int state);

private:
    Ui::MainWindow *ui;

    void setupConnections();
};

#endif // MAINWINDOW_HPP

#ifndef CONNECTIONWINDOW_HPP
#define CONNECTIONWINDOW_HPP

#include <QtCore/qtconfigmacros.h>
#include <QMainWindow>
#include "ChatClient.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class ConnectionWindow;
}
QT_END_NAMESPACE

class ConnectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectionWindow(QWidget *parent = nullptr);
    ~ConnectionWindow();

private slots:
    void onButtonConnectionClicked();

private:
    void setupConnectionWindowUI();
    ChatClient *chatClient;

    Ui::ConnectionWindow *ui;
};

#endif // CONNECTIONWINDOW_HPP

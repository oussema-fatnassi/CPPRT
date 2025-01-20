/**
 * Class ConnectionWindow that represents the connection window of the chat application.
 */

#ifndef CONNECTIONWINDOW_HPP
#define CONNECTIONWINDOW_HPP

#include <QtCore/qtconfigmacros.h>
#include <QMainWindow>
#include "ChatClient.hpp"

// Forward declaration of the Ui::ConnectionWindow class.
QT_BEGIN_NAMESPACE
namespace Ui {
class ConnectionWindow;
}
QT_END_NAMESPACE

// Class ConnectionWindow that represents the connection window of the chat application.
class ConnectionWindow : public QMainWindow
{
    Q_OBJECT

// Public methods are used to connect to the server and handle the connection.
public:
    explicit ConnectionWindow(QWidget *parent = nullptr);
    ~ConnectionWindow();

// Slots are used to handle the connection.
private slots:
    void onButtonConnectionClicked();

// Private methods are used to set up the connection window UI.
private:
    void setupConnectionWindowUI();
    ChatClient *chatClient;

    Ui::ConnectionWindow *ui;
};

#endif // CONNECTIONWINDOW_HPP

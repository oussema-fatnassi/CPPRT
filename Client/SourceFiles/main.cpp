#include "ConnectionWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create and display the connection window
    ConnectionWindow connectionWindow;
    connectionWindow.show();

    return app.exec();
}

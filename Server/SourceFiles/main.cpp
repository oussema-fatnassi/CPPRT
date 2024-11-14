// #include "MainWindow.hpp"

// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }

#include <QCoreApplication>
#include "ChatServer.hpp"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    ChatServer server;
    if (server.start(8080)) {
        return app.exec();
    } else {
        return 1;
    }
}

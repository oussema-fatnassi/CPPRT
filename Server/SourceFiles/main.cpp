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
#include "EnvLoader.hpp"
#include <QFileInfo>


int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    // Load the .env file
    QFileInfo envFileInfo("../../../../../.env");
    QString envFilePath = envFileInfo.absoluteFilePath();

    if (!loadEnvFile(envFilePath)) {
        qDebug() << "Failed to load .env file.";
        return 1;
    }

    QString portStr = qgetenv("CHAT_SERVER_PORT");

    bool portOk;
    quint16 port = portStr.toUShort(&portOk);


    ChatServer server;
    if (server.start(port)) {
        return app.exec();
    } else {
        return 1;
    }
}

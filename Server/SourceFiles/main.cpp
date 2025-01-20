#include "MainWindow.hpp"
#include "ChatServer.hpp"
#include "EnvLoader.hpp"
#include <QApplication>
#include <QFileInfo>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Use QApplication for GUI

    // Load the .env file
    #ifdef _WIN32
        QFileInfo envFileInfo("../../.env");
    #endif

    #ifdef __APPLE__
        QFileInfo envFileInfo("../../../../../.env");
    #endif
    QString envFilePath = envFileInfo.absoluteFilePath();

    if (!loadEnvFile(envFilePath)) {
        qDebug() << "Failed to load .env file.";
        return 1;
    }

    // Get the IP address and port from the environment variables
    QString portStr = qgetenv("CHAT_SERVER_PORT");

    bool portOk;
    quint16 port = portStr.toUShort(&portOk);
    if (!portOk) {
        qDebug() << "Invalid port in .env file.";
        return 1;
    }

    // Start the chat server
    ChatServer server;
    if (!server.start(port)) {
        qDebug() << "Failed to start server.";
        return 1;
    }

    // Create and show the MainWindow
    MainWindow w(&server);
    w.show();

    return app.exec(); // Start the Qt event loop
}

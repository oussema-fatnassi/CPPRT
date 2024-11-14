// #include "MainWindow.hpp"

// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QInputDialog>
#include <QTimer>
#include "ChatClient.hpp"
#include "EnvLoader.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QFileInfo envFileInfo("../../../../../.env");
    QString envFilePath = envFileInfo.absoluteFilePath();

    if (!loadEnvFile(envFilePath)) {
        qDebug() << "Failed to load .env file.";
        return 1;
    }

    QString ipAddress = qgetenv("CHAT_SERVER_IP");
    QString portStr = qgetenv("CHAT_SERVER_PORT");
    bool portOk;
    quint16 port = portStr.toUShort(&portOk);

    if (ipAddress.isEmpty() || !portOk) {
        qDebug() << "Environment variables CHAT_SERVER_IP or CHAT_SERVER_PORT are missing or invalid.";
        return 1;
    }


    QString name = QInputDialog::getText(nullptr, "Enter Name", "Please enter your name:");
    if (name.isEmpty()) {
        qDebug() << "Name cannot be empty.";
        return 1;
    }

    ChatClient client(name);
    client.connectToServer(QHostAddress(ipAddress), port);

    QTimer::singleShot(1000, [&client]() {
        if (client.isConnected()) {
            while (true) {
                QString message = QApplication::arguments().size() > 1
                                      ? QApplication::arguments()[1]
                                      : QInputDialog::getText(nullptr, "Send Message", "Enter message:");
                if (message.isEmpty()) {
                    break;
                }
                client.sendMessage(message);
            }
        } else {
            qDebug() << "Failed to connect to server after delay";
        }
    });

    return app.exec();
}

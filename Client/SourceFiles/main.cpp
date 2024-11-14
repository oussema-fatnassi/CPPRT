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
#include <QInputDialog>
#include <QTimer>
#include "ChatClient.hpp"
#include "QHostAddress"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ChatClient client;
    client.connectToServer(QHostAddress::LocalHost, 8080);

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

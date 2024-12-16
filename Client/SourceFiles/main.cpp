
// #include "MainWindow.hpp"
// #include "WizzEffect.hpp"
// #include "MainWindow.hpp"
// #include "WizzEffect.hpp"

// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();

//     // WizzEffect *wizzEffect = new WizzEffect(&w);  // Corrected line
//     // wizzEffect->start();

//     return a.exec();
// }

// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();

//     WizzEffect *wizzEffect = new WizzEffect(&w);  // Corrected line
//     wizzEffect->start();

//     return a.exec();
// }


#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QInputDialog>
#include <QTimer>
#include "ChatClient.hpp"
#include "EnvLoader.hpp"
#include <QMessageBox>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
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

    QTimer::singleShot(1000, [&client, name]() {
        if (client.isConnected()) {
            while (true) {
                QString message = QInputDialog::getText(
                    nullptr,
                    QString("%1 - Send Message").arg(name),  // Title includes username
                    "Enter message:\n(For DM, use format: DM username message)"
                    );

                if (message.isEmpty()) {
                    break;
                }

                // Check if the message is a DM
                if (message.startsWith("DM ", Qt::CaseInsensitive)) {
                    // Remove the "DM " prefix
                    message = message.mid(3);
                    // Find the first space after the username
                    int spacePos = message.indexOf(' ');
                    if (spacePos > 0) {
                        QString recipient = message.left(spacePos).trimmed();
                        QString dmMessage = message.mid(spacePos + 1).trimmed();
                        if (!recipient.isEmpty() && !dmMessage.isEmpty()) {
                            client.sendDirectMessage(recipient, dmMessage);
                        } else {
                            QMessageBox::warning(nullptr, name + " - Invalid Format",
                                                 "Invalid DM format. Use: DM username message");
                        }
                    } else {
                        QMessageBox::warning(nullptr, name + " - Invalid Format",
                                             "Invalid DM format. Use: DM username message");
                    }
                } else {
                    // Regular broadcast message
                    client.sendMessage(message);
                }
            }
        } else {
            QMessageBox::critical(nullptr, name + " - Connection Error",
                                  "Failed to connect to server after delay");
        }
    });

    return app.exec();
}

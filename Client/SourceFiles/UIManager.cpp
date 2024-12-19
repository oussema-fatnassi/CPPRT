#include "UIManager.hpp"
#include <QTime>

void UIManager::setupTextBrowser(QTextBrowser* textBrowser) {
    textBrowser->setStyleSheet(
        "QTextBrowser {"
        "   background-color: white;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
    );
}

void UIManager::setupTextEdit(QTextEdit* textEdit) {
    textEdit->setStyleSheet(
        "QTextEdit {"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 20px;"
        "   padding: 10px;"
        "   background-color: white;"
        "   margin: 5px;"
        "}"
    );
}

void UIManager::setupButtons(QPushButton* buttonSend, QPushButton* buttonWizz) {
    QString buttonStyle =
        "QPushButton {"
        "   background-color: #0084ff;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding: 8px 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0073e6;"
        "}";

    buttonSend->setStyleSheet(buttonStyle);
    buttonWizz->setStyleSheet(buttonStyle);
}

void UIManager::setupUserList(QListWidget* listClients) {
    listClients->setStyleSheet(
        "QListWidget {"
        "   background-color: white;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item {"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #e6f2ff;"
        "   color: #0084ff;"
        "}"
    );
}

QString UIManager::formatMessage(const QString& message, bool isSentByMe) {
    QString timeStamp = QTime::currentTime().toString("HH:mm");
    QString formattedMessage = replaceEmojis(message);
    
    if (isSentByMe) {
        return QString(
            "<div style='margin: 3px 0;'>"
            "<div style='text-align: left;'>"
            "<div style='background-color: #0078D4; color: white; "
            "width: 100%%; padding: 8px; font-size: 14px;'>"
            "%2</div>"
            "<div style='font-size: 12px; color: #666666; padding: 2px 8px;'>%1</div>"
            "</div>"
            "</div>"
        ).arg(timeStamp).arg(formattedMessage);
    } else {
        return QString(
            "<div style='margin: 3px 0;'>"
            "<div style='text-align: left;'>"
            "<span style='font-weight: bold; color: #333333; padding: 2px 8px;'>%1</span>"
            "<div style='font-size: 12px; color: #666666; padding: 2px 8px;'>%2</div>"
            "<div style='background-color: white; border: 1px solid #E0E0E0; "
            "color: black; width: 100%%; padding: 8px; font-size: 14px;'>"
            "%3</div>"
            "</div>"
            "</div>"
        ).arg("").arg(timeStamp).arg(formattedMessage);
    }
}
QString UIManager::replaceEmojis(QString message) {
    QMap<QString, QString> emojiMap = {
        {":)", "😊"},
        {":-)", "😊"},
        {":(", "😢"},
        {":-(", "😢"},
        {":D", "😄"},
        {":-D", "😄"},
        {"<3", "❤️"},
        {":p", "😛"},
        {":P", "😛"},
        {";)", "😉"},
        {":o", "😮"},
        {":O", "😮"},
        {"xD", "😆"},
        {"XD", "😆"}
    };

    for (auto it = emojiMap.begin(); it != emojiMap.end(); ++it) {
        message.replace(it.key(), it.value());
    }
    return message;
}
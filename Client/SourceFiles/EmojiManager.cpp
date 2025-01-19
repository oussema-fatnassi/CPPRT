#include "EmojiManager.hpp"

EmojiManager::EmojiManager(QObject* parent)
    : QObject(parent) {
    // Define the emojis
    emojis = {"😀", "😂", "❤️", "🎉", "👍", "😢", "😎", "🤔", "🙌", "🔥", "🎶"};
}

QToolButton* EmojiManager::setupEmojiButton(QWidget* parent, int x, int y, int width, int height) {
    // Create the new QToolButton
    QToolButton* toolButton = new QToolButton(parent);
    toolButton->setObjectName("buttonEmoji");
    toolButton->setText("😊"); // Set text or emoji icon
    toolButton->setPopupMode(QToolButton::InstantPopup);
    toolButton->setGeometry(x, y, width, height); // Set position and size

    // Set up the emoji menu
    QMenu* emojiMenu = new QMenu(parent);
    for (const QString& emoji : emojis) {
        QAction* action = emojiMenu->addAction(emoji);
        connect(action, &QAction::triggered, this, [this, emoji]() {
            emit emojiSelected(emoji); // Emit the selected emoji
        });
    }
    toolButton->setMenu(emojiMenu);

    return toolButton;
}

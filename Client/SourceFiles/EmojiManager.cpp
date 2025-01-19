#include "EmojiManager.hpp"
#include <QPushButton>
#include <QMap>
#include <QAction>
#include <QDebug>

EmojiManager::EmojiManager(QObject* parent)
    : QObject(parent) {}

QPushButton* EmojiManager::setupEmojiButton(QWidget* parent, int x, int y, int width, int height) {
    // Create the emoji button
    QPushButton* emojiButton = new QPushButton("😊", parent);
    emojiButton->setFixedSize(width, height);
    emojiButton->setStyleSheet("QPushButton { border: none; }");
    emojiButton->setGeometry(x, y, width, height); // Explicitly set position and size

    // Create the emoji menu with subcategories
    QMenu* emojiMenu = new QMenu(parent);

    // Define emoji categories
    QMap<QString, QStringList> emojiCategories = {
        {"Smileys", {"😊", "😄", "😆", "😅", "😂", "🤣", "😇", "🙂", "😉", "😍"}},
        {"Emotions", {"❤️", "😘", "🥰", "😌", "🤗", "😔", "😢", "😭", "😤", "😠"}},
        {"Gestures", {"👍", "👎", "👋", "✌️", "🤞", "👊", "🤝", "👏", "🙌", "🤲", "🤦"}},
        {"Symbols", {"⭐", "🔥", "💫", "💥", "💢", "💦", "💨", "🕊️", "🎵", "🎶", "💭"}}
    };

    // Populate the menu with categories and emojis
    for (auto it = emojiCategories.begin(); it != emojiCategories.end(); ++it) {
        QMenu* categoryMenu = new QMenu(it.key(), emojiMenu);
        emojiMenu->addMenu(categoryMenu);

        for (const QString& emoji : it.value()) {
            QAction* action = new QAction(emoji, parent);
            connect(action, &QAction::triggered, this, [this, emoji]() {
                emit emojiSelected(emoji); // Emit the selected emoji
            });
            categoryMenu->addAction(action);
        }
    }

    // Connect the button to open the emoji menu
    connect(emojiButton, &QPushButton::clicked, parent, [emojiButton, emojiMenu]() {
        emojiMenu->popup(emojiButton->mapToGlobal(QPoint(0, emojiButton->height())));
    });

    return emojiButton;
}

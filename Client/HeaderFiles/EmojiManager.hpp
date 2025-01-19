#ifndef EMOJIMANAGER_HPP
#define EMOJIMANAGER_HPP

#include <QObject>
#include <QPushButton>
#include <QMenu>
#include <QStringList>

class EmojiManager : public QObject {
    Q_OBJECT

public:
    explicit EmojiManager(QObject* parent = nullptr);

    // Create and return the emoji button with subcategories
    QPushButton* setupEmojiButton(QWidget* parent, int x, int y, int width, int height);

signals:
    void emojiSelected(const QString& emoji);
};

#endif // EMOJIMANAGER_HPP

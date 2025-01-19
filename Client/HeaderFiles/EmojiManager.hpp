#ifndef EMOJIMANAGER_HPP
#define EMOJIMANAGER_HPP

#include <QObject>
#include <QMenu>
#include <QStringList>
#include <QToolButton>

class EmojiManager : public QObject {
    Q_OBJECT

public:
    explicit EmojiManager(QObject* parent = nullptr);
    QToolButton* setupEmojiButton(QWidget* parent, int x, int y, int width, int height);


signals:
    void emojiSelected(const QString& emoji);

private:
    QStringList emojis; // List of available emojis
};

#endif // EMOJIMANAGER_HPP

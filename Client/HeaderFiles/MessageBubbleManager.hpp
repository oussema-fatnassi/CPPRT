/**
 * Class MessageBubbleManager
 * This class is used to manage the message bubbles in the chat application.
 * It handles the painting of the message bubbles and the size hint of the message bubbles.
 */

#ifndef MESSAGEBUBBLEMANAGER_HPP
#define MESSAGEBUBBLEMANAGER_HPP

#include <QStyledItemDelegate>
#include <QPainter>

class MessageBubbleManager : public QStyledItemDelegate {
    Q_OBJECT

// Public methods are used to paint the message bubbles and get the size hint of the message bubbles.
public:
    explicit MessageBubbleManager(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

// Private methods are used to draw the message bubbles.
private:
    void drawMessageBubble(QPainter* painter, const QRect& rect,
                           const QString& text, const QString& timestamp,
                           bool isLocalMessage) const;
};

#endif // MESSAGEBUBBLEMANAGER_HPP

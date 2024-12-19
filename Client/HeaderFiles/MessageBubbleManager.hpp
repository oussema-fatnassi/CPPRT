#ifndef MESSAGEBUBBLEMANAGER_HPP
#define MESSAGEBUBBLEMANAGER_HPP

#include <QStyledItemDelegate>
#include <QPainter>

class MessageBubbleManager : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit MessageBubbleManager(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

private:
    void drawMessageBubble(QPainter* painter, const QRect& rect,
                           const QString& text, const QString& timestamp,
                           bool isLocalMessage) const;
};

#endif // MESSAGEBUBBLEMANAGER_HPP

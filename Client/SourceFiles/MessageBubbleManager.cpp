#include "MessageBubbleManager.hpp"
#include <QPainterPath>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTime>
#include <QTextOption>

MessageBubbleManager::MessageBubbleManager(QObject* parent)
    : QStyledItemDelegate(parent) {}

void MessageBubbleManager::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Get the item data
    QString message = index.data(Qt::DisplayRole).toString();
    QString timestamp = QTime::currentTime().toString("hh:mm");
    bool isLocalMessage = index.data(Qt::UserRole + 1).toBool();

    // Draw the message bubble
    drawMessageBubble(painter, opt.rect, message, timestamp, isLocalMessage);
}

QSize MessageBubbleManager::sizeHint(const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
    // Create a text document to calculate the size
    QTextDocument doc;
    doc.setDefaultFont(option.font);

    // Set text document properties for proper word wrapping
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WordWrap);
    doc.setDefaultTextOption(textOption);

    // Set a maximum width for the text document (adjust this value as needed)
    doc.setTextWidth(option.rect.width() - 60); // Account for padding
    doc.setPlainText(index.data(Qt::DisplayRole).toString());

    // Calculate dimensions
    int textHeight = doc.size().height(); // Full height of the text content
    int timestampHeight = 25;             // Height for the timestamp
    int padding = 40;                     // Total vertical padding

    return QSize(option.rect.width(), textHeight + timestampHeight + padding);
}

void MessageBubbleManager::drawMessageBubble(QPainter* painter, const QRect& rect,
                                             const QString& text, const QString& timestamp,
                                             bool isLocalMessage) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Set up colors
    QColor bubbleColor = isLocalMessage ? QColor(31, 96, 71) : QColor(31, 39, 42);
    QColor textColor = Qt::white; // Ensure text is always black

    // Calculate the maximum width for the bubble
    int maxBubbleWidth = rect.width() * 0.6; // Bubbles take up to 60% of the available width
    int bubblePadding = 10;

    // Create a QTextDocument for text rendering
    QTextDocument doc;
    QFont font = painter->font();
    font.setPointSize(20); // Set desired font size
    doc.setDefaultFont(font);
    doc.setPlainText(text);

    // Apply text options
    doc.setTextWidth(maxBubbleWidth - 20); // Account for padding

    // Apply black text color
    QTextCursor cursor(&doc);
    QTextCharFormat format;
    format.setForeground(textColor);
    cursor.select(QTextCursor::Document);
    cursor.mergeCharFormat(format);

    // Calculate the timestamp width
    QFont timestampFont = painter->font();
    timestampFont.setPointSize(timestampFont.pointSize() - 2); // Smaller font for timestamp
    QFontMetrics timestampMetrics(timestampFont);
    int timestampWidth = timestampMetrics.horizontalAdvance(timestamp) + 20; // Account for padding

    // Calculate the bubble width and height
    int textHeight = static_cast<int>(doc.size().height());
    int timestampHeight = 25; // Height for the timestamp
    int bubbleHeight = textHeight + timestampHeight + 20; // Total bubble height
    int bubbleWidth = qMax(timestampWidth, qMin(maxBubbleWidth, static_cast<int>(doc.idealWidth() + 20)));

    // Determine the bubble rectangle
    QRect bubbleRect;
    if (isLocalMessage) {
        bubbleRect = QRect(rect.right() - bubbleWidth - bubblePadding, rect.top() + bubblePadding,
                           bubbleWidth, bubbleHeight);
    } else {
        bubbleRect = QRect(rect.left() + bubblePadding, rect.top() + bubblePadding,
                           bubbleWidth, bubbleHeight);
    }

    // Create a painter path for the bubble
    QPainterPath bubblePath;
    bubblePath.addRoundedRect(bubbleRect, 15, 15);

    // Draw the bubble
    painter->fillPath(bubblePath, bubbleColor);

    // Draw the message text
    QRect textRect = bubbleRect.adjusted(10, 10, -10, -timestampHeight - 10); // Adjust for padding and timestamp
    painter->translate(textRect.topLeft());
    doc.documentLayout()->draw(painter, QAbstractTextDocumentLayout::PaintContext());
    painter->translate(-textRect.topLeft());

    // Draw the timestamp
    painter->setFont(timestampFont);
    QRect timestampRect = bubbleRect.adjusted(10, bubbleRect.height() - 25, -10, -5);
    painter->setPen(QColor(171, 178, 175)); // Timestamp in black
    painter->drawText(timestampRect, Qt::AlignRight, timestamp);

    painter->restore();
}

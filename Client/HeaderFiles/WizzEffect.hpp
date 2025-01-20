/**
 * WizzEffect class
 * This class is used to create a wizz effect on a widget.
 * It shakes the widget for a specified duration and distance.
 * It is used to notify the user when a wizz is received.
 */

#ifndef WIZZ_EFFECT_HPP
#define WIZZ_EFFECT_HPP

#include <QWidget>
#include <QPoint>
#include <QTimer>

class WizzEffect : public QObject {
    Q_OBJECT

// Public methods are used to start the wizz effect.
public:
    WizzEffect(QWidget *targetWidget, int shakeDistance = 10, int shakeCount = 40, int shakeDuration = 20);
    void start();

// Private slots are used to perform the shake effect.
private slots:
    void performShake();

// Private methods are used to handle the shake effect.
private:
    QWidget *targetWidget;
    QPoint originalPosition;
    QTimer *shakeTimer;
    int shakeDistance;
    int shakeCount;
    int shakeDuration;
    int shakeIndex;
};

#endif // WIZZ_EFFECT_HPP

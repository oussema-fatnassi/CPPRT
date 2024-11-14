#ifndef WIZZ_EFFECT_HPP
#define WIZZ_EFFECT_HPP

#include <QWidget>
#include <QPoint>
#include <QTimer>

class WizzEffect : public QObject {
    Q_OBJECT

public:
    WizzEffect(QWidget *targetWidget, int shakeDistance = 10, int shakeCount = 40, int shakeDuration = 20);
    void start();

private slots:
    void performShake();

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

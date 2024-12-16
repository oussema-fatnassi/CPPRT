#include "WizzEffect.hpp"

WizzEffect::WizzEffect(QWidget *targetWidget, int shakeDistance, int shakeCount, int shakeDuration)
    : targetWidget(targetWidget), shakeDistance(shakeDistance), shakeCount(shakeCount),
    shakeDuration(shakeDuration), shakeIndex(0) {
    shakeTimer = new QTimer(this);
    connect(shakeTimer, &QTimer::timeout, this, &WizzEffect::performShake);
}

void WizzEffect::start() {
    if (!targetWidget) return;

    originalPosition = targetWidget->pos();
    shakeIndex = 0;
    shakeTimer->start(shakeDuration);
}

void WizzEffect::performShake() {
    if (shakeIndex >= shakeCount) {
        shakeTimer->stop();
        targetWidget->move(originalPosition);
        return;
    }

    int offsetX = (shakeIndex % 2 == 0 ? shakeDistance : -shakeDistance);
    int offsetY = (shakeIndex % 4 < 2 ? shakeDistance : -shakeDistance);
    targetWidget->move(originalPosition.x() + offsetX, originalPosition.y() + offsetY);

    ++shakeIndex;
}

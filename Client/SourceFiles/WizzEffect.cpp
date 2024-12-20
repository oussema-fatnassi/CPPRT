#include "WizzEffect.hpp"
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>

WizzEffect::WizzEffect(QWidget *targetWidget, int shakeDistance, int shakeCount, int shakeDuration)
    : targetWidget(targetWidget), shakeDistance(shakeDistance), shakeCount(shakeCount),
    shakeDuration(shakeDuration), shakeIndex(0) {
    
    shakeTimer = new QTimer(this);
    connect(shakeTimer, &QTimer::timeout, this, &WizzEffect::performShake);

    // audio setup
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);

    // test all possible paths
    QStringList possiblePaths = {
        QDir::currentPath() + "/resources/wizz.mp3",
        QDir::currentPath() + "/../resources/wizz.mp3",
        QDir::currentPath() + "/../../resources/wizz.mp3"
    };

    QString soundPath;
    for (const QString &path : possiblePaths) {
        qDebug() << "Testing path:" << path << "Exists:" << QFile::exists(path);
        if (QFile::exists(path)) {
            soundPath = path;
            break;
        }
    }

    if (soundPath.isEmpty()) {
        qDebug() << "ERROR: Could not find wizz.mp3 in any location";
        qDebug() << "Current directory is:" << QDir::currentPath();
        return;
    }

    mediaPlayer->setSource(QUrl::fromLocalFile(soundPath));
    
    // Connect signal to slot
    connect(mediaPlayer, &QMediaPlayer::errorOccurred, 
            this, [](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "MediaPlayer error:" << error << errorString;
    });

    audioOutput->setVolume(1.0);
}

void WizzEffect::start() {
    if (!targetWidget) return;

    originalPosition = targetWidget->pos();
    shakeIndex = 0;
    
    // bugfix: if the widget is not visible, the sound will not play
    qDebug() << "Playing sound...";
    qDebug() << "MediaPlayer state before play:" << mediaPlayer->playbackState();
    qDebug() << "MediaPlayer source:" << mediaPlayer->source().toString();
    qDebug() << "Audio output volume:" << audioOutput->volume();
    
    mediaPlayer->setPosition(0);
    mediaPlayer->play();
    
    qDebug() << "MediaPlayer state after play:" << mediaPlayer->playbackState();
    
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

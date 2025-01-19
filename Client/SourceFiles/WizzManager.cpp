#include "WizzManager.hpp"
#include <QDebug>

WizzManager::WizzManager(QListWidget* clientsList, ChatClient* chatClient,
                         QWidget* effectParent, QObject* parent)
    : QObject(parent), clientsList(clientsList), chatClient(chatClient),
    effectParent(effectParent) {
    wizzSound = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    wizzSound->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);                        // Set maximum volume
    wizzSound->setSource(QUrl("qrc:/resources/wizz.mp3"));

    if (!wizzSound->source().isValid()) {
        qDebug() << "Failed to load sound file!";
        qDebug() << "Source error:" << wizzSound->error();
    }

    connect(wizzSound, &QMediaPlayer::playbackStateChanged, [](QMediaPlayer::PlaybackState state) {
        qDebug() << "Playback state changed to:" << state;
    });

    connect(wizzSound, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString& errorString) {
        qDebug() << "Error occurred during playback:" << error << errorString;
    });
}

void WizzManager::onWizzButtonClicked() {
    QList<QListWidgetItem*> selectedItems = clientsList->selectedItems();
    if (!selectedItems.isEmpty()) {
        QString targetName = selectedItems.first()->text();
        QString wizzCommand = "WIZZ:" + targetName;

        if (chatClient) {
            qDebug() << "Sending WIZZ command to" << targetName;
            chatClient->sendMessage(wizzCommand);
        } else {
            qDebug() << "Chat client not initialized!";
        }
    } else {
        qDebug() << "No user selected!";
    }
}

void WizzManager::triggerWizzEffect() {
    qDebug() << "Wizz effect triggered!";
    if (wizzSound) {
        qDebug() << "Playing sound from source: " << wizzSound->source();

        connect(wizzSound, &QMediaPlayer::playbackStateChanged, [](QMediaPlayer::PlaybackState state) {
            qDebug() << "Playback state changed to:" << state;
        });

        connect(wizzSound, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString& errorString) {
            qDebug() << "Error occurred during playback:" << error << errorString;
        });

        wizzSound->play();
    } else {
        qDebug() << "wizzSound is null!";
    }

    WizzEffect* wizzEffect = new WizzEffect(effectParent);
    wizzEffect->start();
}


void WizzManager::setChatClient(ChatClient* chatClient) {
    this->chatClient = chatClient;
    qDebug() << "ChatClient set in WizzManager!";
}

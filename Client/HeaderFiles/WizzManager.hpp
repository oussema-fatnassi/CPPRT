#ifndef WIZZMANAGER_HPP
#define WIZZMANAGER_HPP

#include <QObject>
#include <QListWidget>
#include "ChatClient.hpp"
#include "WizzEffect.hpp"
#include <QMediaPlayer>
#include <QAudioOutput>

class WizzManager : public QObject {
    Q_OBJECT

public:
    explicit WizzManager(QListWidget* clientsList, ChatClient* chatClient,
                         QWidget* effectParent, QObject* parent = nullptr);
    void setChatClient(ChatClient* chatClient);


public slots:
    void onWizzButtonClicked();
    void triggerWizzEffect();

private:
    QListWidget* clientsList;
    ChatClient* chatClient;
    QWidget* effectParent;
    QMediaPlayer* wizzSound;
    QAudioOutput* audioOutput;
};

#endif // WIZZMANAGER_HPP

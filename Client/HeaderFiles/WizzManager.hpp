/**
 * WizzManager class
 * This class is responsible for managing the wizz effect.
 * It triggers the wizz effect when the wizz button is clicked.
 */

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

// Public methods are used to set the chat client and trigger the wizz effect.
public:
    explicit WizzManager(QListWidget* clientsList, ChatClient* chatClient,
                         QWidget* effectParent, QObject* parent = nullptr);
    void setChatClient(ChatClient* chatClient);

// Slots are used to handle the wizz button click event and trigger the wizz effect.
public slots:
    void onWizzButtonClicked();
    void triggerWizzEffect();

// Private methods are used to handle the wizz effect.
private:
    QListWidget* clientsList;
    ChatClient* chatClient;
    QWidget* effectParent;
    QMediaPlayer* wizzSound;
    QAudioOutput* audioOutput;
};

#endif // WIZZMANAGER_HPP

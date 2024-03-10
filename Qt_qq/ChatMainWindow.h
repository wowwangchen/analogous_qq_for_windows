#ifndef CHATMAINWINDOW_H
#define CHATMAINWINDOW_H

#include <QMainWindow>
#include<QVBoxLayout>
#include<QTreeWidget>
#include<QVBoxLayout>
#include<QDebug>
#include"theFriendMessage.h"
#include<QScrollBar>
#include<QKeyEvent>
#include"ChatLeftWidget.h"
#include<QListWidgetItem>
#include<QMultimedia>
#include<QMediaPlayer>
#include<QAudioOutput>

namespace Ui {
class ChatMainWindow;
}

typedef struct listItem
{
    QListWidgetItem* item;
    theFriendMessage* widget;
}ListItem;
typedef struct listMessageItem
{
    QListWidgetItem* item;
    ChatLeftWidget* widget;
}ListMessageItem;

class ChatMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatMainWindow(QWidget *parent = nullptr);
    ~ChatMainWindow();
    void setAllStyleSheet();         //设置界面的样式表
    void initChat();                 //初始化消息列表
    void initFriends();              //初始化好友列表
    void keyPressEvent(QKeyEvent *event);
    void initMessageChatList();             //初始化聊天窗口
signals:
    void exitWindow();      //退出
    void ctrlEnterPressed();//ctrl+enter=发送消息

private slots:
    void on_friendsButton_clicked();

    void on_chatButton_clicked();

    void on_messageTextEdit_textChanged();

    void on_sendMessageButton_clicked();

    void on_chatListWidget1_itemClicked(QListWidgetItem *item);

    void on_photoButton_clicked();

private:
    Ui::ChatMainWindow *ui;


    QVector<ListItem> _chatItems;               //聊天大纲显示
    QVector<ListMessageItem> _chatListItems;   //主聊天窗口
    QMediaPlayer* musicPlayer;
    bool isPlaying;
};

#endif // CHATMAINWINDOW_H

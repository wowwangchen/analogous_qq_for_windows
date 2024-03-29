﻿#ifndef CHATMAINWINDOW_H
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
#include"ChatRightWidget.h"
#include<QListWidgetItem>
#include<QMultimedia>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QTcpSocket>
#include<QHostAddress>
#include"ListItemDelegate.h"
#include<QStringListModel>
#include<QString>
#include<QTimer>  //延时
#include<QPixmap> //截图
#include<QScreen>
#include<QClipboard>
#include<QThread>
#include<QMessageBox>
#include"ListItemModel.h"
#include"People.h"
#include<QFile>
#include<QFileDialog>
#include<QFileInfo>
#include"AddFriend.h"
#include"EmojiModel.h"
#include<QMouseEvent>
#include"EmojiDelegate.h"
#include<QTextToSpeech>
#include"MyTextToSpeech.h"
#include<QVoice>
#include"MyAudio.h"
#include<QDir>
#include"codeTranslate.h"
#include"ToUTF_8.h"

namespace Ui {
class ChatMainWindow;
}

typedef struct listItem
{
    QListWidgetItem* item;
    theFriendMessage* widget;
}ListItem;

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
    void connectToServer(QString accountName); //连接到服务端
    void sendNameToServer(QString accountName);  //客户端连接到服务端时，通过发送一个writeline发送自己的account名字
    void initMyself(QString account,QVector<QString> friends);
        void addFriend(QString account); //添加好友
    bool judgeIsInFriendList(QString mightPeople);
        void setAllConnect();
signals:
    void exitWindow();      //退出
    void ctrlEnterPressed();//ctrl+enter=发送消息
    void sendMessage(QString mes);
    void getMessage(QString mes);
protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_friendsButton_clicked();

    void on_chatButton_clicked();

    void on_messageTextEdit_textChanged();

    void on_sendMessageButton_clicked();

    void on_chatListWidget1_itemClicked(QListWidgetItem *item);

    void on_photoButton_clicked();

    void onConnect();           //打印连接到服务端后这个自己的socket的ip和port
    void onSocketReadyRead();   //客户端要接受消息了
    void on_cutButton_clicked();
    void ScreenShot();  //截屏

    void on_friendsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_selectFileButton_clicked();

    void on_addFriendButton_clicked();

    void on_faceButton_clicked();
    void on_emojiView_clicked(const QModelIndex &index);

    void on_chatMessageListView_doubleClicked(const QModelIndex &index);



    void on_sendVoiceButton_pressed();

    void on_sendVoiceButton_released();

    void on_saveChatBox_activated(int index);

private:
    Ui::ChatMainWindow *ui;


    QVector<ListItem> _chatItems;               //聊天大纲显示
    QMediaPlayer* musicPlayer;//音乐播放器，小插件
    bool isPlaying;           //音乐是否在播放
    QTcpSocket* _socket;      //当前这个客户端的套接字

    QString beAcceptedAccount;   //此时要发送消息的被接收者

    QMap<QString,QStringListModel*> Messagemodel;
    ListItemDelegate *MessageDelegate;
    AddFriend* addFriendWidget;     //添加好友的页面

    People* mySelf;         //自己的所有信息

    QVector<QTreeWidgetItem*> friendsItems;  //好友列表中的项

    QString filePath;    //文件路径
    EmojiModel *emojiModel; //表情的listview
    QListView *emojiView;
    EmojiDelegate *emojiDelegate;

    MyTextToSpeech *m_speech;

    MyAudio* m_audio;


};

#endif // CHATMAINWINDOW_H

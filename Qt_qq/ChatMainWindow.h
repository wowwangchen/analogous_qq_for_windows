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
#include"ChatRightWidget.h"
#include<QListWidgetItem>
#include<QMultimedia>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QTcpSocket>
#include<QHostAddress>
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
}ListMessageItem1;
typedef struct listMessageItem2
{
    QListWidgetItem* item;
    ChatRightWidget* widget;
}ListMessageItem2;

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
    void sendNameToServer(QString accountName);                   //客户端连接到服务端时，通过发送一个writeline发送自己的account名字
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

    void onConnect();           //打印连接到服务端后这个自己的socket的ip和port
    void onSocketReadyRead();   //客户端要接受消息了

private:
    Ui::ChatMainWindow *ui;


    QVector<ListItem> _chatItems;               //聊天大纲显示
    QVector<ListMessageItem1> _chatListItems1;   //主聊天窗口
     QVector<ListMessageItem2> _chatListItems2;   //主聊天窗口
    QMediaPlayer* musicPlayer;//音乐播放器，小插件
    bool isPlaying;           //音乐是否在播放
    QTcpSocket* _socket;      //当前这个客户端的套接字
    //QString accountName;      //登录的这个账号的名字
    QString beAcceptedAccount;   //此时要发送消息的被接收者
};

#endif // CHATMAINWINDOW_H

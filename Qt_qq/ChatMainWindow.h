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
namespace Ui {
class ChatMainWindow;
}

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
    void initChatList();             //初始化聊天窗口
signals:
    void exitWindow();      //退出
    void ctrlEnterPressed();//ctrl+enter=发送消息





private slots:
    void on_friendsButton_clicked();

    void on_chatButton_clicked();

    void on_messageTextEdit_textChanged();

    void on_sendMessageButton_clicked();

private:
    Ui::ChatMainWindow *ui;
};

#endif // CHATMAINWINDOW_H

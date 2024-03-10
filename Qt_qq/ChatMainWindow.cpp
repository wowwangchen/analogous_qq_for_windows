#include "ChatMainWindow.h"
#include "ui_ChatMainWindow.h"

ChatMainWindow::ChatMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatMainWindow)
{
    ui->setupUi(this);
    setAllStyleSheet();
    initChat();
    initMessageChatList();
    //initFriends();

    QObject::connect(musicPlayer, &QMediaPlayer::stateChanged, [&](QMediaPlayer::State state) {
        if (state == QMediaPlayer::PlayingState) {
            // 音乐正在播放
            qDebug() << "Music is playing";
        } else if (state == QMediaPlayer::StoppedState) {
            // 音乐已停止播放
            qDebug() << "Music stopped";
        }
    });


    connect(_socket,SIGNAL(connected()),this,SLOT(onConnect()));
}

ChatMainWindow::~ChatMainWindow()
{
    //释放内存
    for (ListItem& listItem : _chatItems)
    {
        delete listItem.item;
        delete listItem.widget;
    }
    _chatItems.clear();

    for (ListMessageItem& listItem : _chatListItems)
    {
        delete listItem.item;
        delete listItem.widget;
    }
    _chatListItems.clear();

    delete musicPlayer;
    delete ui;
    delete _socket;
    emit exitWindow();
}

void ChatMainWindow::setAllStyleSheet()
{
    //图标
    QIcon iconIcon(":/picture/icon.jpg");
    setWindowIcon(iconIcon);
    setWindowTitle(" ");

    //搜索栏
    ui->searchLineEdit->setPlaceholderText("🔍搜索");
    ui->searchLineEdit->setAlignment(Qt::AlignLeft);   //左侧

    //左侧导航栏
    ui->chatButton->move(QPoint(5,10));


    //头像
    QIcon photoIcon(":/picture/icon.jpg");
    ui->photoButton->setIcon(photoIcon);
    ui->photoButton->setIconSize(ui->photoButton->size());
    //设置聊天、好友、空间、文件、设置的图标
    QIcon Icon1(":/picture/chatblue.png");
    ui->chatButton->setIcon(Icon1);
    ui->chatButton->setIconSize(ui->chatButton->size());

    QIcon Icon2(":/picture/friends.png");
    ui->friendsButton->setIcon(Icon2);
    ui->friendsButton->setIconSize(ui->friendsButton->size());

    QIcon Icon3(":/picture/space.png");
    ui->spaceButton->setIcon(Icon3);
    ui->spaceButton->setIconSize(ui->spaceButton->size());

    QIcon Icon4(":/picture/file.png");
    ui->fileButton->setIcon(Icon4);
    ui->fileButton->setIconSize(ui->fileButton->size());

    QIcon Icon5(":/picture/setting.png");
    ui->settingButton->setIcon(Icon5);
    ui->settingButton->setIconSize(ui->settingButton->size());

    //stackwidget首先默认设置为消息列表
    ui->stackedWidget->setCurrentIndex(0);

    //发送消息
    QObject::connect(this, &ChatMainWindow::ctrlEnterPressed, ui->sendMessageButton, &QPushButton::click);
    ui->sendMessageButton->setEnabled(false);

    //右边栏填充
    //QPixmap rightSidePixmap(":/picture/rightSide.jpg");
    //ui->rightSideLabel->setPixmap(rightSidePixmap);
    //ui->rightSideLabel->setScaledContents(true);

    //播放音乐
    musicPlayer=new QMediaPlayer(this);
    musicPlayer->setMedia(QUrl("qrc:/music/seeYouAgain.mp3"));
    isPlaying=false;
    musicPlayer->setVolume(50);


    //socket
    _socket=new QTcpSocket;


}

void ChatMainWindow::initChat()
{

    QListWidgetItem* item=new QListWidgetItem(ui->chatListWidget1);
    item->setSizeHint(QSize(ui->friendsTreeWidget->width(),90));
    ui->chatListWidget1->addItem(item);
    theFriendMessage*item1=new theFriendMessage;
    item1->setName("zhangbo");
    ui->chatListWidget1->setItemWidget(item,item1);
    ListItem temp;
    temp.item=item;
    temp.widget=item1;
    _chatItems.push_back(temp);



    QListWidgetItem* item2=new QListWidgetItem(ui->chatListWidget1);
    item2->setSizeHint(QSize(240,90));
    ui->chatListWidget1->addItem(item2);
    theFriendMessage*item3=new theFriendMessage;
    item3->setName("zhouyang");
    ui->chatListWidget1->setItemWidget(item2,item3);
    ListItem temp2;
    temp2.item=item2;
    temp2.widget=item3;
    _chatItems.push_back(temp2);
}

void ChatMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Return)
    {
        emit ctrlEnterPressed();
    }
}

void ChatMainWindow::initMessageChatList()
{
}

void ChatMainWindow::connectToServer(QString accountName)
{
    //测试一下，所以用本地ip
    _socket->connectToHost("127.0.0.1",50660);   //客户端连接到服务端的ip+port

    sendNameToServer();
}

void ChatMainWindow::sendNameToServer()
{
    QByteArray str=accountName.toUtf8();
    str.append('\n');
    _socket->write(str);
}



//void ChatMainWindow::initFriends()
//{
//    QTreeWidgetItem *group1=new QTreeWidgetItem(ui->friendsTreeWidget);
//    group1->setText(0, "分组1");
//    QTreeWidgetItem *friend1 = new QTreeWidgetItem(group1);
//    friend1->setText(0, "好友1");

//    QTreeWidgetItem *group2=new QTreeWidgetItem(ui->friendsTreeWidget);
//    group2->setText(0, "分组2");
//    QTreeWidgetItem *friend2 = new QTreeWidgetItem(group2);
//    friend2->setText(0, "好友3");
//    QTreeWidgetItem *friend3 = new QTreeWidgetItem(group2);
//    friend3->setText(0, "好友4");

//    ui->friendsTreeWidget->addTopLevelItem(group1);
//    ui->friendsTreeWidget->addTopLevelItem(group2);
//}



void ChatMainWindow::on_chatButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); //好友列表

    //切换聊天和好友按钮的颜色
    QIcon Icon1(":/picture/chatblue.png");
    ui->chatButton->setIcon(Icon1);
    ui->chatButton->setIconSize(ui->chatButton->size());
    QIcon Icon2(":/picture/friends.png");
    ui->friendsButton->setIcon(Icon2);
    ui->friendsButton->setIconSize(ui->friendsButton->size());
}


void ChatMainWindow::on_friendsButton_clicked()
{
    qDebug()<<"111";
    ui->stackedWidget->setCurrentIndex(1); //好友列表
    //initFriends();

    QIcon Icon1(":/picture/chat.png");
    ui->chatButton->setIcon(Icon1);
    ui->chatButton->setIconSize(ui->chatButton->size());
    QIcon Icon2(":/picture/friendsblue.png");
    ui->friendsButton->setIcon(Icon2);
    ui->friendsButton->setIconSize(ui->friendsButton->size());
}





void ChatMainWindow::on_messageTextEdit_textChanged()
{
    //不为空,可以发送消息
    if(ui->messageTextEdit->toPlainText()=="")
    {
        ui->sendMessageButton->setStyleSheet("QPushButton {"
                                             "font: 10pt \"Segoe Print\";"
                                             "background-color: rgb(0, 153, 255);"
                                             "color: rgb(77, 184, 255);"
                                             "border-radius: 10px;"
                                             "padding: 10px;"
                                             "}"
                                             "QPushButton:pressed {"
                                             "background-color: rgb(0, 134, 224);"
                                             "}");
        ui->sendMessageButton->setEnabled(false);
    }
    else
    {
        ui->sendMessageButton->setStyleSheet("QPushButton {"
                                             "font: 10pt \"Segoe Print\";"
                                             "background-color: rgb(0, 153, 255);"
                                             "color: rgb(255,255, 255);"
                                             "border-radius: 10px;"
                                             "padding: 10px;"
                                             "}"
                                             "QPushButton:pressed {"
                                             "background-color: rgb(0, 134, 224);"
                                             "}");
        ui->sendMessageButton->setEnabled(true);
    }
}

void ChatMainWindow::on_chatListWidget1_itemClicked(QListWidgetItem *item)
{
    theFriendMessage* widget=static_cast<theFriendMessage*>(ui->chatListWidget1->itemWidget(item));
    if(widget!=nullptr)
    {
        ui->nameLabel->setText(widget->getName());
        beAcceptedAccount=widget->getName();
    }
}


void ChatMainWindow::on_photoButton_clicked()
{
    qDebug()<<"1";
    if(isPlaying==false)
        musicPlayer->play();
    else
        musicPlayer->pause();
    isPlaying=!isPlaying;
}

void ChatMainWindow::onConnect()
{
    qDebug()<<"**Peer Address:"+_socket->peerAddress().toString()+"**";
    qDebug()<<"**Peer Port:"+QString::number(_socket->peerPort())+"**";
}



void ChatMainWindow::on_sendMessageButton_clicked()
{
    //获取message
    QString message=ui->messageTextEdit->toPlainText();
    ui->messageTextEdit->clear();

    //设置item-widget
    QListWidgetItem* chatItem=new QListWidgetItem(ui->chatMessageListWidget);
    chatItem->setSizeHint(QSize(ui->chatMessageListWidget->width(),90));
    chatItem->setFlags(chatItem->flags() & ~Qt::ItemIsSelectable);
    ui->chatMessageListWidget->addItem(chatItem);

    ChatLeftWidget*chatWidget=new ChatLeftWidget;
    chatWidget->setMessage(message);
    chatWidget->setSize(message.size());
    ui->chatMessageListWidget->setItemWidget(chatItem,chatWidget);

    //添加到vector，方便最后释放内存
    ListMessageItem temp;
    temp.item=chatItem;
    temp.widget=chatWidget;
    _chatListItems.push_back(temp);

    //设置焦点最新状态
    ui->chatMessageListWidget->scrollToBottom();
    ui->messageTextEdit->setFocus();

    QByteArray str=message.toUtf8();
    QByteArray name=beAcceptedAccount.toUtf8();
    str=name+"###"+str;
    str.append('\n');
    _socket->write(str);

    qDebug() << "Message sent to server: " << message;
}


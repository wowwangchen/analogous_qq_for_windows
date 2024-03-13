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

    //    for (QTreeWidgetItem* item : friendsItems)
    //    {
    //        delete item; // 释放内存
    //        item = nullptr; // 将指针设置为nullptr
    //    }
    friendsItems.clear();

    delete musicPlayer;
    delete ui;
    delete _socket;
    delete MessageDelegate;

    for (QStringListModel* model : Messagemodel)
    {
        delete model;
    }
    Messagemodel.clear();

    delete mySelf;
    emit exitWindow();
}

void ChatMainWindow::setAllStyleSheet()
{
    //图标
    QIcon iconIcon(":/picture/icon.jpg");
    setWindowIcon(iconIcon);

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
    connect(_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));


    //view的model与delegate

    MessageDelegate=new ListItemDelegate(ui->chatMessageListView);
    ui->chatMessageListView->setItemDelegate(MessageDelegate);


    this->mySelf=new People("-1");


}

void ChatMainWindow::initChat()
{

    QListWidgetItem* item=new QListWidgetItem(ui->chatListWidget1);
    item->setSizeHint(QSize(ui->friendsTreeWidget->width(),90));
    ui->chatListWidget1->addItem(item);
    theFriendMessage*item1=new theFriendMessage;
    item1->setName("zhoumi");
    ui->chatListWidget1->setItemWidget(item,item1);
    ListItem temp;
    temp.item=item;
    temp.widget=item1;
    _chatItems.push_back(temp);



    QListWidgetItem* item2=new QListWidgetItem(ui->chatListWidget1);
    item2->setSizeHint(QSize(240,90));
    ui->chatListWidget1->addItem(item2);
    theFriendMessage*item3=new theFriendMessage;
    item3->setName("1");
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

    sendNameToServer(accountName);
}

void ChatMainWindow::sendNameToServer(QString accountName)
{
    QByteArray str=accountName.toUtf8();
    str.append('\n');
    _socket->write(str);

}

void ChatMainWindow::initMyself(QString account,QVector<QString> friends)
{
    this->mySelf->account=account;
    this->mySelf->friends=friends;
    setWindowTitle(mySelf->account+"----USER");
    initFriends();
}

void ChatMainWindow::ScreenShot()
{
    QPixmap *pixmap=new QPixmap;
    *pixmap=QGuiApplication::primaryScreen()->grabWindow(0);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(*pixmap);

    *pixmap=pixmap->scaled(ui->messageTextEdit->width()/4,ui->messageTextEdit->height()/2);
    QTextCursor cursor = ui->messageTextEdit->textCursor();
    QTextDocument *document = ui->messageTextEdit->document();
    // 在光标位置插入图像
    document->addResource(QTextDocument::ImageResource, QUrl("screenshot"), *pixmap);
    cursor.insertImage("screenshot");

    this->show();
    //delete pixmap;

}



void ChatMainWindow::initFriends()
{
    QTreeWidgetItem* firstItem =new QTreeWidgetItem(ui->friendsTreeWidget); // 获取第一个顶级项目
    friendsItems.push_back(firstItem);
    firstItem->setText(0,"朋友");
    ui->friendsTreeWidget->addTopLevelItem(firstItem);

    qDebug()<<mySelf->friends.size();
    for (int i=0;i<mySelf->friends.size();i++)
    {
        QString friendName=mySelf->friends[i];
        QTreeWidgetItem* item = new QTreeWidgetItem(firstItem);
        item->setText(0, friendName); // 在第一列中设置文本
        friendsItems.push_back(item);
        // 可以设置更多属性和列的文本
        firstItem->addChild(item);
    }
}



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
    }

    QStringListModel* _model=Messagemodel[ui->nameLabel->text()];
    if(_model==nullptr)
    {
        QStringListModel* model=new QStringListModel;
        Messagemodel[ui->nameLabel->text()]=model;
        _model=model;
    }
    ui->chatMessageListView->setModel(_model);
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

void ChatMainWindow::onSocketReadyRead()
{
    QString sender="";
    QString message=_socket->readLine();
    qDebug()<<"message"<<message;

    //取出发消息的人的account和消息
    while(_socket->canReadLine())
    {
        message+=_socket->readLine();
    }
    while (!message.isEmpty() && message.endsWith('\n'))
        message = message.chopped(1); // 移除末尾的换行符

    //发送文件
    if(message=="###ok")
    {
        qDebug()<<"ok";
        QFile file(filePath);
        if (!file.open(QIODevice::ReadWrite))
        {
            //读取文件失败
            return;
        }
        qint64 currentlen = 0;//当前已经发送的大小
        qint64 allLength = file.size();//总文件大小
        do
        {
            char data[1024];
            qint64 msize = file.read(data, 1024);//读文件放入打他数组中，返回读取到的大小
            _socket->write(data, msize);//把读取到的data数据发送给服务器
            currentlen += msize;//实时获取当前发送的文件大小
        } while (currentlen < allLength);//当发送文件等于文件大小时，发送完毕，循环结束

        return;

    }

    QStringList parts = message.split("###");
    qDebug()<<"parts:"<<parts;
    if(parts.size()==2)
    {
        sender=parts[0];
        message=parts[1];

        //让delegate识别，左对齐
        message="a1`"+message;

        //
        //通过名字来map出对应的model显示它们的聊天记录
        QStringListModel* _model=Messagemodel[sender];
        if(_model==nullptr)
        {
            QStringListModel* model=new QStringListModel;
            Messagemodel[sender]=model;
            _model=model;
        }
        //ui->chatMessageListView->setModel(_model);
        QStringList itemlist = _model->stringList();
        itemlist.append(message);
        _model->setStringList(itemlist);
    }

    else
    {
        parts=message.split("{{{}}}");
        sender=parts[0];
        QString filePath=parts[1];
        filePath="{{{}}}"+filePath;

        QStringListModel* _model=Messagemodel[sender];
        if(_model==nullptr)
        {
            QStringListModel* model=new QStringListModel;
            Messagemodel[sender]=model;
            _model=model;
        }

        QStringList itemlist = _model->stringList();
        itemlist.append(filePath);
        _model->setStringList(itemlist);
    }

    ui->chatMessageListView->update();


}



void ChatMainWindow::on_sendMessageButton_clicked()
{
    QString message = ui->messageTextEdit->toPlainText();
    ui->messageTextEdit->clear();

    // 添加并更新model指向的stringlist
    //每个人对应着一个model
    QStringListModel* _model=Messagemodel[ui->nameLabel->text()];
    if(_model==nullptr)
    {
        QStringListModel* model=new QStringListModel;
        Messagemodel[ui->nameLabel->text()]=model;
        _model=model;
    }
    ui->chatMessageListView->setModel(_model);
    QStringList itemlist = _model->stringList();
    itemlist.append(message);
    _model->setStringList(itemlist);


    ui->chatMessageListView->update();


    //设置焦点最新状态
    ui->chatMessageListView->scrollToBottom();
    ui->messageTextEdit->setFocus();

    QByteArray str=message.toUtf8();
    QByteArray name=ui->nameLabel->text().toUtf8();
    str=name+"###"+str;
    str.append('\n');
    _socket->write(str);

    //qDebug() << "Message sent to server: " << message;
}


void ChatMainWindow::on_cutButton_clicked()
{
    this->hide();
    QTimer::singleShot(1000, this, SLOT(ScreenShot()));
}


void ChatMainWindow::on_friendsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current &&!current->parent())
        return;
    ui->nameLabel->setText(current->text(0));

    QStringListModel* _model=Messagemodel[ui->nameLabel->text()];
    if(_model==nullptr)
    {
        QStringListModel* model=new QStringListModel;
        Messagemodel[ui->nameLabel->text()]=model;
        _model=model;
    }
    ui->chatMessageListView->setModel(_model);
}


void ChatMainWindow::on_selectFileButton_clicked()
{
    qDebug()<<"on_selectFileButton_clicked";

    filePath = QFileDialog::getOpenFileName(nullptr, "选择文件", "", "ALL FILES(*.*)");

    //如果文件不为空，则继续
    if (!filePath.isEmpty())
    {
        // 进行文件传输逻辑处理
        QFile file(filePath);
        //获取文件的类型
        QFileInfo fileInfo(filePath);
        //file###高三###100###zhoumi
        qDebug()<<"size:"<<fileInfo.size();
        QString head="file###"+fileInfo.fileName()+"###"+QString::number(fileInfo.size())+"###"+ui->nameLabel->text();
        qDebug()<<head;
        _socket->write(head.toUtf8());



        QStringListModel* _model=Messagemodel[ui->nameLabel->text()];
        if(_model==nullptr)
        {
            QStringListModel* model=new QStringListModel;
            Messagemodel[ui->nameLabel->text()]=model;
            _model=model;
        }
        ui->chatMessageListView->setModel(_model);
        QStringList itemlist = _model->stringList();
        itemlist.append("已发送文件:"+fileInfo.fileName());
        _model->setStringList(itemlist);


    }
    else
    {
        qDebug()<<"file is empty";
    }
}


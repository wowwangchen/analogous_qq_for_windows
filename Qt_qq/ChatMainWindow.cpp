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
    //接收消息包
    QString str=_socket->readAll();
    qDebug()<<str;

    while (!str.isEmpty() && str.endsWith('\n'))
        str = str.chopped(1); // 移除末尾的换行符

    QStringList strlist = str.split("###");


    //文本消息
    if(strlist[3]=="0")
    {
        QString sender=strlist[1];
        QString message=strlist[4];

        //让delegate识别，左对齐
        message="a1`"+message;


        //通过名字来map出对应的model显示它们的聊天记录
        QStringListModel* _model=Messagemodel[sender];
        if(_model==nullptr)
        {
            QStringListModel* model=new QStringListModel;
            Messagemodel[sender]=model;
            _model=model;
        }
        QStringList itemlist = _model->stringList();
        itemlist.append(message);
        _model->setStringList(itemlist);
    }
    //文件
    else if(strlist[3]=="1")
    {
        QString sender=strlist[1];
        QMessageBox::information(this,"Tips","you accept file from user----"+strlist[1]+":"+strlist[5]);

        filePath = QFileDialog::getExistingDirectory(nullptr, "文件保存路径","/");
        if (!filePath.endsWith("/"))
        {
            filePath += "/";
        }
        QFile file(filePath+strlist[5]);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(nullptr, "Tips", "save file fail!");
        }
        else
        {
            file.write(strlist[6].toUtf8());
            file.close();
            QMessageBox::information(nullptr, "Tips", "save file success!");
        }

        //通过名字来map出对应的model显示它们的聊天记录
        QStringListModel* _model=Messagemodel[sender];
        if(_model==nullptr)
        {
            QStringListModel* model=new QStringListModel;
            Messagemodel[sender]=model;
            _model=model;
        }
        QStringList itemlist = _model->stringList();
        itemlist.append("a1`get file from user----"+sender+":"+strlist[5]);
        _model->setStringList(itemlist);

    }

    ui->chatMessageListView->update();
}



void ChatMainWindow::on_sendMessageButton_clicked()
{
    //消息数据包
    QString str;
    QString sign="0";
    QString sender=mySelf->account;
    QString accepter=ui->nameLabel->text();
    QString messageType="0";
    QString message = ui->messageTextEdit->toPlainText();

    //    0         1           2               3                   4
    str=sign+"###"+sender+"###"+accepter+"###"+messageType+"###"+message;
    ui->messageTextEdit->clear();
    qDebug()<<str;

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

    _socket->write(str.toUtf8());

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
    // 文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(0) + 数据
    // 表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(0) + 表情个数 + images + 数据
    // 文件数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(1) + 文件长度 +文件名称 + 文件内容

    //消息数据包
    QString str;
    QString sign="0";
    QString sender=mySelf->account;
    QString accepter=ui->nameLabel->text();
    QString messageType="1";
    QString messageLength;
    QString fileName;
    QString message;



    filePath = QFileDialog::getOpenFileName(nullptr, "选择文件", "", "ALL FILES(*.*)");

    //如果文件不为空，则继续
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {

            //获取文件的相关信息
            QFileInfo fileInfo(filePath);

            messageLength=QString::number(fileInfo.size());
            fileName=fileInfo.fileName();
            message=file.readAll();
            //    0         1           2               3                   4                   5           6
            str=sign+"###"+sender+"###"+accepter+"###"+messageType+"###"+messageLength+"###"+fileName+"###"+message;
            qDebug()<<"filename:"<<fileName;
            _socket->write(str.toUtf8());



            QStringListModel* _model=Messagemodel[ui->nameLabel->text()];
            if(_model==nullptr)
            {
                QStringListModel* model=new QStringListModel;
                Messagemodel[ui->nameLabel->text()]=model;
                _model=model;
            }
            ui->chatMessageListView->setModel(_model);
            QStringList itemlist = _model->stringList();
            itemlist.append("send file to user----"+accepter+":"+fileInfo.fileName());
            _model->setStringList(itemlist);
        }

    }
    else
    {
        qDebug()<<"file is empty";
    }
}


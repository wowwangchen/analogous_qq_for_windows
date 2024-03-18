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
    setAllConnect();
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
    delete emojiModel;
    delete emojiDelegate;
    emojiView->close();
    addFriendWidget->close();

    //发送的消息
    for (QStringListModel* model : Messagemodel)
    {
        delete model;
    }
    Messagemodel.clear();


    //好友列表
    for (QTreeWidgetItem* item : friendsItems)
    {
        delete item;
    }
    friendsItems.clear();

    delete mySelf;
    delete m_audio;

    emit exitWindow();
}

void ChatMainWindow::setAllStyleSheet()
{
    //图标
    QIcon iconIcon(":/picture/icon.jpg");
    setWindowIcon(iconIcon);

    //搜索栏
    ui->searchLineEdit->setPlaceholderText(QStringLiteral("搜索"));
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

    //view的model与delegate
    MessageDelegate=new ListItemDelegate(ui->chatMessageListView);
    ui->chatMessageListView->setItemDelegate(MessageDelegate);
    ui->chatMessageListView->setToolTip("double clicked to read");
    ui->chatMessageListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //个人信息
    this->mySelf=new People("-1");

    //按钮提示
    ui->selectFileButton->setToolTip("send file");
    ui->selectPictureButton->setToolTip("send picture");
    ui->faceButton->setToolTip("select emoji");
    ui->cutButton->setToolTip("screenShot");
    ui->addFriendButton->setToolTip("add friend");
    ui->sendVoiceButton->setToolTip("tap and hold(voice to text)");

    //添加好友的页面
    addFriendWidget=new AddFriend();
    addFriendWidget->hide();


    //表情
    emojiModel=new EmojiModel;
    emojiView=new QListView(this);
    emojiDelegate=new EmojiDelegate;
    emojiView->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    emojiView->setModel(emojiModel);
    emojiView->setItemDelegate(emojiDelegate);
    emojiView->setFlow(QListView::LeftToRight); // 设置项的流动方向为从左到右
    emojiView->setWrapping(true); // 启用项的换行显示
    emojiView->hide();


    foreach (QString engine, QTextToSpeech::availableEngines())
        qDebug()<<engine;


    //语音播放功能
    m_speech=new MyTextToSpeech;
    //m_speech->set

    //m_speech
    QVector<QVoice> voices = m_speech->availableVoices();
    //qDebug()<<"size:"<<voices.size();
    for(auto iter : voices)
    {
        //qDebug() << "Checking voice: " << iter.name()<<' '<<iter.age()<<" "<<iter.gender();
        if(iter.age()==QVoice::Age::Child&&iter.gender()==QVoice::Gender::Male)
        {
            m_speech->setVoice(iter);
        }
    }

    //发送语音功能
    m_audio=new MyAudio;
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


//这里初始化了好友列表
void ChatMainWindow::initMyself(QString account,QVector<QString> friends)
{
    this->mySelf->account=account;
    this->mySelf->friends=friends;
    setWindowTitle(mySelf->account+"----USER");
    initFriends();   //这里初始化了好友列表
}

void ChatMainWindow::addFriend(QString account)
{
    QTreeWidgetItem* firstItem = ui->friendsTreeWidget->topLevelItem(0);
    if (firstItem)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(firstItem);
        item->setText(0, account);
        firstItem->addChild(item);

        friendsItems.push_back(item);
    }
}

bool ChatMainWindow::judgeIsInFriendList(QString mightPeople)
{
    for (int i=0;i<mySelf->friends.size();i++)
    {
        QString friendName=mySelf->friends[i];
        if(friendName==mightPeople)
            return true;  //在里面
    }
    return false;
}

void ChatMainWindow::setAllConnect()
{
    QObject::connect(musicPlayer, &QMediaPlayer::stateChanged, [&](QMediaPlayer::State state) {
        if (state == QMediaPlayer::PlayingState) {
            // 音乐正在播放
            qDebug() << "Music is playing";
        } else if (state == QMediaPlayer::StoppedState) {
            // 音乐已停止播放
            qDebug() << "Music stopped";
        }
    });


    connect(addFriendWidget,&AddFriend::addFriend,this,[=]()mutable{
        //消息数据包
        QString str;
        QString sign="0";
        QString sender=mySelf->account;
        QString accepter;
        QString messageType="3";
        bool flag=true;
        qDebug()<<"进入了@@@@@@@@@@@";

        accepter=addFriendWidget->getAccount();
        if(accepter==sender)
        {
            QMessageBox::information(this,"Tips","You can't add your own as a friend");
            flag=false;
            //return;
        }
        else if(judgeIsInFriendList(accepter)==true)
        {
            QMessageBox::information(this,"Tips","the people is your friend, no need to add twice");
            flag=false;
            //return;
        }

        if(flag)
        {
            str=sign+"###"+sender+"###"+accepter+"###"+messageType;
            _socket->write(str.toUtf8());
            addFriendWidget->hide();
        }
    });

    //socket Read
    connect(_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    //socket connect
    connect(_socket,SIGNAL(connected()),this,SLOT(onConnect()));
    //click emoji
    connect(this->emojiView,&QListView::clicked,this,&ChatMainWindow::on_emojiView_clicked);
}

void ChatMainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    if (!this->emojiView->viewport()->rect().contains(pos))
        this->emojiView->hide();

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
    //父节点，分组
    QTreeWidgetItem* firstItem =new QTreeWidgetItem(ui->friendsTreeWidget); // 获取第一个顶级项目
    friendsItems.push_back(firstItem);
    firstItem->setText(0,"朋友");
    ui->friendsTreeWidget->addTopLevelItem(firstItem);


    //qDebug()<<"mySelf->friends.size()"<<mySelf->friends.size();
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



    //文本消息:收消息
    if(strlist[3]=="0")
    {
        QString sender=strlist[1];
        QString message=strlist[4];

        //让delegate识别，左对齐
        message="```"+message;


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
        qDebug()<<message;
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


    else if(strlist[3]=="3")
    {
        //要加别人的人
        if(strlist[0]=="0")
        {
            //判断找没找到
            if(strlist[4]=="true")
            {
                QMessageBox::information(this,"Tips","add successfully");
                mySelf->friends.push_back(strlist[2]);
                addFriend(strlist[2]);
            }
            else if(strlist[4]=="false")
            {
                QMessageBox::information(this,"Tips","add failed");
            }

        }
        //别人加自己
        else if(strlist[0]=="1")
        {
            //如果str[0]是1的话代表是别人加自己，肯定这两个人都存在，只有判断是否已经是自己的好友即可
            if(judgeIsInFriendList(strlist[1])==false)   //想加自己的人不是自己的好友
            {
                QMessageBox::information(this,"Tips","you add a friend----\""+strlist[1]+"\"");
                mySelf->friends.push_back(strlist[1]);
                addFriend(strlist[1]);
            }

        }
        return;
    }





    ui->chatMessageListView->update();
}



void ChatMainWindow::on_sendMessageButton_clicked()
{
    if(ui->nameLabel->text().isEmpty())
    {
        return;
    }
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
    // 加好友数据包格式：是要加的人(0)还是被加的人(1) + 发消息员工QQ号 + 被加员工QQ号 + 信息类型(3) +是否找到


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
            //qDebug()<<"filename:"<<fileName;
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


void ChatMainWindow::on_addFriendButton_clicked()
{
    addFriendWidget->show();
}



void ChatMainWindow::on_faceButton_clicked()
{
    QPoint buttonPos = ui->faceButton->mapToGlobal(ui->faceButton->rect().topLeft());
    emojiView->setGeometry(buttonPos.x(), buttonPos.y() - 10, 200, 100);
    emojiView->setFixedSize(QSize(ui->chatMessageListView->width()*2.0/3.0,ui->chatMessageListView->height()*2.0/3.0));
    emojiView->setIconSize(QSize(emojiView->width()/5.0,emojiView->height()/5.0));
    emojiView->show();
}

void ChatMainWindow::on_emojiView_clicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        QVariant data = index.data(Qt::DisplayRole);
        if (data.isValid())
        {
            QString emoji = data.toString();
            QTextCursor cursor = ui->messageTextEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            cursor.insertText(emoji);
            ui->messageTextEdit->setTextCursor(cursor);
            //ui->messageTextEdit->append(emoji);

            this->emojiView->hide();
        }
    }
}


void ChatMainWindow::on_chatMessageListView_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        QVariant data = index.data(Qt::DisplayRole);
        if (data.isValid())
        {
            QString message = data.toString();
            m_speech->say(message);
        }
    }
}





void ChatMainWindow::on_sendVoiceButton_pressed()
{
    this->m_audio->startAudio("F:/test.pcm");
}


void ChatMainWindow::on_sendVoiceButton_released()
{
    this->m_audio->stopAudio();

    QString retStr=m_audio->startSpeech();
    ui->messageTextEdit->append(retStr);
}




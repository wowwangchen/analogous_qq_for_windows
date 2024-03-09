#include "ChatMainWindow.h"
#include "ui_ChatMainWindow.h"

ChatMainWindow::ChatMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatMainWindow)
{
    ui->setupUi(this);
    setAllStyleSheet();
    initChat();
    initChatList();
    //initFriends();


}

ChatMainWindow::~ChatMainWindow()
{
    delete ui;
    emit exitWindow();
}

void ChatMainWindow::setAllStyleSheet()
{
    //图标
    QIcon iconIcon(":/picture/icon.jpg");
    setWindowIcon(iconIcon);
    setWindowTitle(" ");

    //搜索栏
    ui->searchLineEdit->setPlaceholderText("🔍搜索                ");
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
}

void ChatMainWindow::initChat()
{
    QListWidgetItem* item=new QListWidgetItem(ui->chatListWidget);
    item->setSizeHint(QSize(ui->friendsTreeWidget->width(),90));
    ui->chatListWidget->addItem(item);
    theFriendMessage*item1=new theFriendMessage;
    item1->setName("zhangbo");
    ui->chatListWidget->setItemWidget(item,item1);

    QListWidgetItem* item2=new QListWidgetItem(ui->chatListWidget);
    item2->setSizeHint(QSize(240,90));
    ui->chatListWidget->addItem(item2);
    theFriendMessage*item3=new theFriendMessage;
    item3->setName("zhouyang");
    ui->chatListWidget->setItemWidget(item2,item3);
}

void ChatMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Return)
    {
        emit ctrlEnterPressed();
    }
}

void ChatMainWindow::initChatList()
{
    QListWidgetItem* item=new QListWidgetItem(ui->chatMessageListWidget);
    item->setSizeHint(QSize(ui->chatMessageListWidget->width(),100));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable); //不可点击
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->chatMessageListWidget->addItem(item);
    theFriendMessage*item1=new theFriendMessage;
    item1->setName("zhangbo");
    ui->chatMessageListWidget->setItemWidget(item,item1);

    QListWidgetItem* item2=new QListWidgetItem(ui->chatMessageListWidget);
    item2->setSizeHint(QSize(ui->chatMessageListWidget->width(),100));
    item2->setFlags(item2->flags() & ~Qt::ItemIsSelectable);
    ui->chatMessageListWidget->addItem(item2);
    theFriendMessage*item3=new theFriendMessage;
    item3->setName("zhouyang");
    ui->chatMessageListWidget->setItemWidget(item2,item3);

    QListWidgetItem* item4=new QListWidgetItem(ui->chatMessageListWidget);
    item4->setSizeHint(QSize(ui->chatMessageListWidget->width(),100));
    item4->setFlags(item4->flags() & ~Qt::ItemIsSelectable);
    ui->chatMessageListWidget->addItem(item4);
    theFriendMessage*item5=new theFriendMessage;
    item5->setName("zhouyang");
    ui->chatMessageListWidget->setItemWidget(item4,item5);

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
                                             "color: rgb(228, 244, 255);"
                                             "border-radius: 10px;"
                                             "padding: 10px;"
                                             "}"
                                             "QPushButton:pressed {"
                                             "background-color: rgb(0, 134, 224);"
                                             "}");
        ui->sendMessageButton->setEnabled(true);
    }
}


void ChatMainWindow::on_sendMessageButton_clicked()
{
    QString message=ui->messageTextEdit->toPlainText();
    ui->messageTextEdit->clear();

    QListWidgetItem* item4=new QListWidgetItem(ui->chatMessageListWidget);
    item4->setSizeHint(QSize(ui->chatMessageListWidget->width(),100));
    item4->setFlags(item4->flags() & ~Qt::ItemIsSelectable);
    ui->chatMessageListWidget->addItem(item4);
    theFriendMessage*item5=new theFriendMessage;
    item5->setName(message);
    ui->chatMessageListWidget->setItemWidget(item4,item5);

    ui->chatMessageListWidget->scrollToBottom();
    ui->messageTextEdit->setFocus();

    //QScrollBar *scrollBar = ui->scrollArea->verticalScrollBar();
    //scrollBar->setValue(scrollBar->minimum());
}


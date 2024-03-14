#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    initDatabase();
    server=new QTcpServer;
    ishead=true;
    isFileMessage=false;
    setServer();

    QTimer timer;
    timer.setInterval(500);
    QObject::connect(&timer, &QTimer::timeout,this,[=](){setServer();});
    timer.start();

    QObject::connect(server, &QTcpServer::newConnection, [&]() {
        CLIENTINFO clientInfo;

        QTcpSocket *clientSocket;
        clientSocket = server->nextPendingConnection();

        clientInfo.clientSocket=clientSocket;
        clientInfo.accountName="111";
        clientInfo.isFirstConnect=true;
        _clients.push_back(clientInfo);


        qDebug() << "New client connected"+QString::number(_clients.size());



        //connect(clientSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
        connect(clientSocket,&QTcpSocket::readyRead,[=](){
            for (CLIENTINFO& _client : _clients)
            {
                if(clientSocket==_client.clientSocket)
                {
                    onSocketReadyRead(_client);
                    break;
                }
            }
        });
    });

}

MainWidget::~MainWidget()
{
    for (CLIENTINFO& _client : _clients)
    {
        delete _client.clientSocket;
    }
    _clients.clear();

    delete server;
    delete ui;
}

void MainWidget::setServer()
{
    qDebug()<<"in set server";
    QHostAddress addr("127.0.0.1");
    if (server->listen(addr, 50660))
    {
        qDebug() << "Server started";
    } else
    {
        qDebug() << "Server failed to start";
    }

}



void MainWidget::onSocketReadyRead(CLIENTINFO& clientInfo1)
{
    if(clientInfo1.isFirstConnect==true)
    {
        QString mightName=clientInfo1.clientSocket->readLine();
        if (mightName.endsWith('\n'))
            mightName = mightName.chopped(1); // 移除末尾的一个字符（换行符）
        clientInfo1.accountName=mightName;

//        for (CLIENTINFO& _client : _clients)
//        {
//            qDebug()<<_client.accountName;
//        }

        clientInfo1.isFirstConnect=false; //之后全都是false
        return;
    }

    QString str=clientInfo1.clientSocket->readAll();
    QStringList strlist =str.split("###");

    //从数据库中找好友
    if(str.startsWith("###addPeople###"))
    {
        qq_account.open();
        str.remove(0,15); //剩下的就是要查找的账号名
        qDebug()<<"str:"<<str;
        QSqlQuery query;
        QString queryString = QString("SELECT account FROM account_password WHERE account = '%1'")
                                  .arg(str);
        query.exec(queryString);

        if (query.next())
        {
            str="###addPeople###true"+str;
            clientInfo1.clientSocket->write(str.toUtf8());
            qDebug() << "find the people!";
        }
        else
        {
            // 没有找到匹配的数据
            str="###addPeople###false"+str;
            clientInfo1.clientSocket->write(str.toUtf8());
            qDebug() << "not find the people!";
        }
        qq_account.close();
        return;
    }

    // 文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(0) + 数据
    // 表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(0) + 表情个数 + images + 数据
    // 文件数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(1) + 文件长度 +文件名称 + 文件内容

    //消息类型为0:文本消息
    if(strlist[3]=="0"||strlist[3]=="1")
    {

        ui->textEdit->append(strlist[1]+" send message to "+strlist[2]);

        //接下来就是发消息到服务端
        sendToAccepter(str);
    }


    //    0         1           2               3                   4                   5           6
    //str=sign+"###"+sender+"###"+accepter+"###"+messageType+"###"+messageLength+"###"+fileName+"###"+message;
    //消息类型为1：文件
}



void MainWidget::sendToAccepter(QString str)
{
    bool havegetAccepter=false;
    QStringList strlist =str.split("###");
    for(int i=0;i<_clients.size();i++)
    {
        CLIENTINFO clientInfo=_clients.at(i);

        if(clientInfo.accountName==strlist[2])
        {
            clientInfo.clientSocket->write(str.toUtf8());

        }
        if(havegetAccepter) return;
    }


}

void MainWidget::sendFileToAccepter(QString str)
{
    bool havegetAccepter=false;
    QStringList strlist =str.split("###");
    for(int i=0;i<_clients.size();i++)
    {
        CLIENTINFO clientInfo=_clients.at(i);

        if(clientInfo.accountName==strlist[2])
        {
            clientInfo.clientSocket->write(str.toUtf8());
        }
        if(havegetAccepter) return;
    }
}

void MainWidget::initDatabase()
{
    qq_account=QSqlDatabase::addDatabase("QMYSQL");
    qq_account.setHostName("localhost");//本地主机IP和"127.0.0.1"一样
    qq_account.setPort(3306);//默认端口
    qq_account.setDatabaseName("qq_account");//连接的数据库
    qq_account.setUserName("root");//用户
    QString position1="wc";
    QString position2="love0";
    int num=812;
    QString position3=position1+position2+QString::number(num)+".";
    qq_account.setPassword("wclove0812.");//数据库密码
    //注意:需要在MySQL中先创建一个数据库才可以

    if (qq_account.open())
    {
        qDebug()<<"connect Success!!!";
    }
    else
    {
        qDebug()<<"connect falied!!!";
    }
    qq_account.close();
}


#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    server=new QTcpServer;
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

        qDebug() << "New client connected";

        connect(clientSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
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

//    while (true)
//    {
//        if (server.waitForNewConnection(-1))
//        {
//            QTcpSocket* clientSocket = server.nextPendingConnection();
//            qDebug() << "New client connected";

//            connect(clientSocket, &QTcpSocket::readyRead, [=]() {
//                QByteArray data = clientSocket->readAll();
//                QString message = QString::fromUtf8(data);
//                qDebug() << "Message received from client:" << message;
//            });

//            connect(clientSocket, &QTcpSocket::disconnected, [=]() {
//                qDebug() << "Client disconnected";
//                clientSocket->deleteLater();
//            });
//        }
    //    }
}


void MainWidget::onSocketReadyRead()
{
    bool haveGetPeople=false;
    //接受者和内容
    QString recipient;
    QString content;

    //qDebug()<<4444;
    //遍历所有客户端，找到那个发送消息的客户端就退出
    for(int i=0;i<_clients.size();i++)
    {
        qDebug()<<"_client.size:"<<_clients.size();
        CLIENTINFO &_clientInfo=_clients[i];
        //遍历所有客户端，有的话就遍历那一个的消息，然后退出
        //消息分为被接受者的名字+真正的消息
        while(_clientInfo.clientSocket->canReadLine())
        {

            //第一次连接会顺带着一个发送用户名的消息，如果是第一次连接就赋予用户名的值
            if(_clientInfo.isFirstConnect==true)
            {
                QString mightName=_clientInfo.clientSocket->readLine();
                if (mightName.endsWith('\n'))
                    mightName = mightName.chopped(1); // 移除末尾的一个字符（换行符）
                _clientInfo.accountName=mightName;

                _clientInfo.isFirstConnect=false; //之后全都是false
                qDebug()<<_clientInfo.accountName+"_clientInfo.accountName";
                return;
            }



            QString message =_clientInfo.clientSocket->readLine();
            QStringList parts = message.split("###");
            if (parts.size() == 2)
            {
                recipient = parts[0];
                content = parts[1];
                ui->textEdit->append(_clientInfo.accountName+" send message to "+recipient);
                haveGetPeople=true;

                //接下来就是发消息到服务端
                sendMessageToAccepter(recipient,content);
            }

        }
        if(haveGetPeople) break;

    }
}
void MainWidget::sendMessageToAccepter(QString accepter, QString message)
{
    bool haveGetPeople=false;

    foreach(auto _clientInfo,_clients)
    {
        if(_clientInfo.accountName==accepter)
        {
            haveGetPeople=true;
            //服务端发消息

            QByteArray str=message.toUtf8();
            str.append('\n');
            _clientInfo.clientSocket->write(str);
        }
        if(haveGetPeople) break;
    }
}


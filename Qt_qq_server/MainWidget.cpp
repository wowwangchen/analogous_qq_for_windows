#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setServer();

    QObject::connect(server, &QTcpServer::newConnection, [&]() {
        CLIENTINFO clientInfo;

        QTcpSocket *clientSocket;
        clientSocket = server->nextPendingConnection();

        clientInfo.clientSocket=clientSocket;
        clientInfo.accountName="111";
        clientInfo.isFirstConnect=false;
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
    server=new QTcpServer;
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
    qDebug()<<"!11";
    bool haveGetPeople=false;
    QString acceptedPeople;

    //遍历所有客户端，找到那个发送消息的客户端就退出
    foreach(auto _clientInfo,_clients)
    {

        //第一次连接会顺带着一个发送用户名的消息，如果是第一次连接就赋予用户名的值
        if(_clientInfo.isFirstConnect)
        {
            _clientInfo.accountName=_clientInfo.clientSocket->readLine();
            _clientInfo.isFirstConnect=false; //之后全都是false
            break;
        }

        //遍历所有客户端，有的话就遍历那一个的消息，然后退出
        //消息分为被接受者的名字+真正的消息
        while(_clientInfo.clientSocket->canReadLine())
        {
            QString message =_clientInfo.clientSocket->readLine();
            QStringList parts = message.split("###");

            if (parts.size() == 2)
            {
                QString recipient = parts[0];
                QString content = parts[1];

                // 在这里使用接收者名称和消息内容进行进一步处理
                // ...
                acceptedPeople=parts.at(0);
                qDebug()<<acceptedPeople;

                ui->textEdit->append("message sent to \""+recipient+"\":"+parts.at(1));
                 haveGetPeople=true;
            }
        }
        if(haveGetPeople) break;
    }

}


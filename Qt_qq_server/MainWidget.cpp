#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

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

            qDebug()<<"mutable";
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
    QString content;

    if(clientInfo1.isFirstConnect==true)
    {
        QString mightName=clientInfo1.clientSocket->readLine();
        if (mightName.endsWith('\n'))
            mightName = mightName.chopped(1); // 移除末尾的一个字符（换行符）
        clientInfo1.accountName=mightName;

        for (CLIENTINFO& _client : _clients)
        {
            qDebug()<<_client.accountName;
        }

        clientInfo1.isFirstConnect=false; //之后全都是false
        qDebug()<<clientInfo1.accountName+"_clientInfo.accountName";
        return;
    }


    QByteArray buf =clientInfo1.clientSocket->readAll();
    QString message=QString(buf);
    //qDebug()<<"buf:"<<buf;
    //qDebug()<<"message:"<<message;


    //如果是头并且是file###开头
    if (ishead==true&&isFileMessage==false&&message.startsWith("file###"))
    {
        QStringList strlist = message.split("###");
        qDebug()<<strlist;
        fileName = strlist.at(1);               //解析帧头文件名
        fileSize = strlist.at(2).toInt();           //解析帧头文件大小
        ishead = false;//下次接收到的文件就是我们的数据
        recvSize=0;    //当前接收的文件大小
        filebuf.clear();
        recipient=strlist.at(3);
        clientInfo1.clientSocket->write("###ok");
        isFileMessage=true;
        return;
    }

    //不是头，是文件信息
    else if(ishead==false&&isFileMessage==true)
    {
        filebuf.append(buf);  //二进制存储
        recvSize += buf.size();//每接收一次文件，当前文件大小+1
        //当接收文件大小等于总文件大小，即文件数据接收完毕
        if (recvSize>=fileSize)
        {
            //保存文件
            //sendFileToAccepter(recipient,filebuf,_clientInfo1.accountName);
            isFileMessage=false;
            ishead=true;

            sendFileToAccepter(recipient,filebuf,clientInfo1.accountName);

            QFile file("F:/"+fileName);
            file.open(QIODevice::WriteOnly);
            file.write(filebuf);
            file.close();
            return;
        }
    }

    //不是文件信息
    else if(isFileMessage==false)
    {
        qDebug()<<"##########3";
        QStringList parts = message.split("###");
        if (parts.size() == 2)
        {
            recipient = parts[0];
            content = parts[1];

            ui->textEdit->append(clientInfo1.accountName+" send message to "+recipient);

            //接下来就是发消息到服务端
            sendMessageToAccepter(recipient,content,clientInfo1.accountName);
        }
    }
}



void MainWidget::sendMessageToAccepter(QString accepter, QString message,QString sender)
{
    bool havegetAccepter=false;
    for(int i=0;i<_clients.size();i++)
    {
        CLIENTINFO clientInfo=_clients.at(i);

        if(clientInfo.accountName==accepter)
        {
            qDebug()<<"getit";
            QTcpSocket* socket=clientInfo.clientSocket;

            QString str=sender+"###"+message;
            socket->write(str.toUtf8());
            havegetAccepter=true;
        }
        if(havegetAccepter) return;
    }


}

void MainWidget::sendFileToAccepter(QString accepter, QByteArray message, QString sender)
{
    bool havegetAccepter=false;
    for(int i=0;i<_clients.size();i++)
    {
        CLIENTINFO clientInfo=_clients.at(i);

        if(clientInfo.accountName==accepter)
        {
            QTcpSocket* socket=clientInfo.clientSocket;

            QString str=sender+"{{{}}}F:/"+fileName;  //将文件路径发送过去
            socket->write(str.toUtf8());
            havegetAccepter=true;
        }
        if(havegetAccepter) return;
    }
}


#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QHostInfo>
#include<QTimer>
#include<QByteArray>
#include<QFile>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

typedef struct clientInfo
{
    QString accountName;
    QTcpSocket* clientSocket;
    bool isFirstConnect;
}CLIENTINFO;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void setServer();
    void sendToAccepter(QString str);
    void sendFileToAccepter(QString str);
public slots:
    void onSocketReadyRead(CLIENTINFO& clientInfo);

private:
    Ui::MainWidget *ui;
    QTcpServer* server;
    //QTcpSocket* clientSocket;
    QVector<CLIENTINFO> _clients;
    bool ishead;
    bool isFileMessage;

    QString fileName;
    int fileSize;//接收文件的总大小
    int recvSize;//当前接收文件的大小
    QByteArray filebuf;//当前接收的文件数据

    QString recipient;
};
#endif // MAINWIDGET_H

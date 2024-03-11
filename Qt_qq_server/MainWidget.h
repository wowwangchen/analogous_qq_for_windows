#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QHostInfo>
#include<QTimer>
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
    void sendMessageToAccepter(QString accepter,QString message);
public slots:
    void onSocketReadyRead();

private:
    Ui::MainWidget *ui;
    QTcpServer* server;
    //QTcpSocket* clientSocket;
    QVector<CLIENTINFO> _clients;
};
#endif // MAINWIDGET_H

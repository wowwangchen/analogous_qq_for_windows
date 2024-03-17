#ifndef HTTP_H
#define HTTP_H

#include <QObject>

#include <QNetworkAccessManager> //发送请求
#include <QNetworkRequest>              //请求内容
#include <QNetworkReply>                //返回的结果

#include <QEventLoop>

class Http : public QObject
{
    Q_OBJECT
public:
    explicit Http(QObject *parent = nullptr);
    static bool http_postRequst(QString Url, QMap<QString,QString>header,
                                    QByteArray &requestData, QByteArray &replyData);

signals:

};

#endif // HTTP_H

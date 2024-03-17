#ifndef SPEECH_H
#define SPEECH_H


#include <http.h>
#include <QObject>
#include <QByteArray>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QHostInfo>
#include <QDebug>
#include"Http.h"
const QString BaiduSpeechUrl = "http://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2"; //获取token请求的Url
const QString BaiduSpeechClientID = "v0jaeF9X3Z7e0n5TsHaD5keZ";                                 //AK
const QString BaiduSpeechClientSecret = "Sm3pQ1ERXLu11RlRddnlpRBsHBdUNvVn";     //SK

//语音识别的Url
const QString BaiduSpeechSatrtUrl = "http://vop.baidu.com/pro_api?dev_pid=80001&cuid=%1&token=%2"; //80001识别普通话  %1:本机标识 %2:获取的token
/*
 * 1.发送获取token值的Url，相当于向百度对接，获取通行证(token值)
 * 2.发送语音识别的Url，进行语音识别
 */



class Speech : public QObject
{
    Q_OBJECT
public:
    explicit Speech(QObject *parent = nullptr);
    QString speechIdentify(QString audioFile);//语音识别请求函数

private:
    QString getJsonValue(QByteArray &data, QString &key);//json解析函数

private:
    QString accessToken;//存放获取的token值
};

#endif // SPEECH_H

﻿#include "Speech.h"

Speech::Speech(QObject *parent)
    : QObject{parent}
{
    qDebug()<<QSslSocket::sslLibraryBuildVersionString();

    QNetworkAccessManager *manager=new QNetworkAccessManager(this);
    qDebug()<<"manager->supportedSchemes"<<manager->supportedSchemes();
}

QString Speech::speechIdentify(QString audioFile)
{
    if(audioFile.isEmpty())
    {
        QMessageBox::warning(NULL,"警告(speech)","the file not exist");
        return QString("");
    }
    bool ret = false;



    /* 组装access token的Url */
    QString TokenUrl = QString(BaiduSpeechUrl).arg(BaiduSpeechClientID).arg(BaiduSpeechClientSecret);



    /**************获取token值不用的参数，但自定义http函数需要传入**************/
    QMap<QString, QString>header; //Content-Type: audio/pcm;rate=16000键值对       RAW方式上传音频
    header.insert(QString("Content-Type"),QString("audio/pcm;rate=16000"));

    QByteArray requestData;//存放上传的录音信息
    QByteArray replyData;//存放语音识别返回的结果
    /**************获取token值不用的参数，但自定义http函数需要传入**************/


    //获取token值，获取一次使用30天
    if(accessToken.isEmpty() == true)//如果token值为空，即未获取token值
    {
        //http请求，以获取token值
        ret = Http::http_postRequst(TokenUrl, header, requestData, replyData);
        if(ret)
        {
            QString key = "access_token";
            accessToken = getJsonValue(replyData,key);
            replyData.clear();
            qDebug() << "access_token ----" << accessToken;
        }
    }


    /* 将获取的token值组装到新的url中用于发送语音识别请求*/
    //QString speechUrl=QString(BaiduSpeechSatrtUrl).arg(QHostInfo::localHostName()).arg(accessToken);
    QString speechUrl=QString(BaiduSpeechSatrtUrl).arg("98-43-FA-95-18-48").arg(accessToken);

    /* 把文件转换成QByteArray */
    QFile file;
    file.setFileName(audioFile);//获取保存的录音文件
    file.open(QIODevice::ReadOnly);
    requestData = file.readAll();//读取录音文件中的内容
    file.close();

    if(requestData.isEmpty())
    {
        return QString("the data is empty");
    }

    /* 再次发送http请求       进行语音识别 */
    ret = Http::http_postRequst(speechUrl, header, requestData, replyData);
    if(ret)
    {
        QString key = "result";
        QString text = getJsonValue(replyData,key);//json解析，自定义函数将结果转换成QString
        return text;
        qDebug()<<"true";
    }
    else
    {
        QMessageBox::warning(NULL,"warning(speech)","Recognition failure");
    }
    return QString("");

}

QString Speech::getJsonValue(QByteArray &data, QString &key)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data,&parseError);
    QString retStr = "";
    if(parseError.error == QJsonParseError::NoError)
    {
        if(jsonDocument.isObject())
        {
            /*  将jsonDocument 转换成json对象 */
            QJsonObject jsonObj = jsonDocument.object();
            if(jsonObj.contains(key))
            {
                QJsonValue jsonVal = jsonObj.value(key);
                if(jsonVal.isString())
                {
                    return jsonVal.toString();
                }
                else if(jsonVal.isArray())//检查是否为数组
                {
                    QJsonArray arr = jsonVal.toArray();
                    for(int index = 0; index<arr.size(); index++)
                    {
                        QJsonValue subValue = arr.at(index);
                        if(subValue.isString())
                        {
                            retStr += subValue.toString()+" ";
                        }
                    }
                    qDebug() <<"data:"<< data.data();
                    return retStr;
                }
            }
            else
            {
                qDebug() << "Keywords are not included:" << key;
            }//contains(key)
        }
        else
        {
            qDebug() << "Not a json object";
        }//isObject
    }
    else
    {
        qDebug() << "Failed to parse json";
    }//NoError
    qDebug() <<"data:"<< data.data();
        return QString("");

}

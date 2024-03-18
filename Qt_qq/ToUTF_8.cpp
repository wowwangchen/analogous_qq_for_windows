#include "ToUTF_8.h"

ToUTF_8::ToUTF_8(QObject *parent)
    : QObject{parent}
{

}

UTF8Status ToUTF_8::file_is_utf8(QFile &file)
{


    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool firstIsBOM = false;
        UTF8Status result = UTF8Status::UTF8_NOBOM;
        QByteArray contentBytes = file.readAll();
        for (int i = 0; i < contentBytes.size();)
        {
            uchar ch = contentBytes.at(i);
            // ASCII字符
            if (ch < 0x80)
            {
                i++;
                continue;
            }
            // 无效的UTF8字符
            else if (ch < 0xC0)
            {
                result = UTF8Status::UTF8_NOT;
                //qDebug()<<"result = UTF8Status::UTF8_NOT1";
                break;
            }
            // 2字节的UTF8字符
            else if (ch < 0xE0)
            {
                if (i >= contentBytes.size() - 1)
                {
                    result = UTF8Status::UTF8_Unknown;
                    //qDebug()<<"result = UTF8Status::UTF8_Unknown";
                    break;
                }
                if ((contentBytes.at(i + 1) & 0xC0) != 0x80)
                {
                    result = UTF8Status::UTF8_NOT;
                    //qDebug()<<"result = UTF8Status::UTF8_Unknown2";
                    break;
                }
                i += 2;
            }
            // 3字节UTF8字符
            else if (ch < 0xF0)
            {
                if (i >= contentBytes.size() - 2)
                {
                    result = UTF8Status::UTF8_Unknown;
                    //qDebug()<<"result = UTF8Status::UTF8_Unknown";
                    break;
                }
                if ((contentBytes.at(i + 1) & 0xC0) != 0x80 || (contentBytes.at(i + 2) & 0xC0) != 0x80)
                {
                    result = UTF8Status::UTF8_NOT;
                    //qDebug()<<"result = UTF8_NOT";
                    break;
                }
                // 是否为BOM
                if (i == 0)
                {
                    uchar ch1 = contentBytes.at(i + 1);
                    uchar ch2 = contentBytes.at(i + 2);
                    if (ch == 0xEF && ch1 == 0xBB && ch2 == 0xBF)
                    {
                        firstIsBOM = true;
                    }
                }
                i += 3;
            }
            // 非法
            else
            {
                result = UTF8Status::UTF8_NOT;
                break;
            }
        }
        file.close();
        if (UTF8Status::UTF8_Unknown == result || UTF8Status::UTF8_NOT == result)
        {
            return result;
        }
        return firstIsBOM ? UTF8Status::UTF8_BOM : UTF8Status::UTF8_NOBOM;
    }
    return UTF8Status::UTF8_Unknown;

}


void ToUTF_8::convert_file(QString filepath)
{
    QFile temp(filepath);
    UTF8Status result = file_is_utf8(temp);
    qDebug()<<"result:#######"<<result;
    if (UTF8Status::UTF8_Unknown == result || UTF8Status::UTF8_BOM == result)
    {
        return;
    }
    QString contentString;
    QFile file(filepath);





   /******************************读出**************************************/
    // 以GBK编码读出
    if (result == UTF8Status::UTF8_NOT)
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("gbk");
            contentString = stream.readAll();
            file.close();
        }
    }
    // 以UTF8编码读出
    else
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("utf8");
            contentString = stream.readAll();
            file.close();
        }
    }
     /******************************读出**************************************/





    /******************************转换成utf8写入************************** ***/
    // 以UTF8编码写入
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray contentBytes = contentString.toUtf8();
        // 前三个字节是BOM
        char bom[3];
        bom[0] = 0xEF;
        bom[1] = 0xBB;
        bom[2] = 0xBF;
        contentBytes = contentBytes.insert(0, bom, 3);
        // 写入文件
        file.write(contentBytes);
        file.close();
    }
    /******************************转换成utf8写入************************** ***/


}

#ifndef TOUTF_8_H
#define TOUTF_8_H

#include <QObject>
#include<QFile>
#include<QTextStream>
#include<qDebug>
typedef enum {
    UTF8_Unknown,   //0
    UTF8_NOT,       //1
    UTF8_BOM,       //2
    UTF8_NOBOM      //3
}UTF8Status;

class ToUTF_8 : public QObject
{
    Q_OBJECT
public:
    explicit ToUTF_8(QObject *parent = nullptr);
    UTF8Status file_is_utf8(QFile& file);
    void convert_file(QString filepath);

signals:

};

#endif // TOUTF_8_H

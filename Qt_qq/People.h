#ifndef PEOPLE_H
#define PEOPLE_H

#include<QIcon>
class People
{
public:
    People(QString account="-1");

    QIcon photo;            //头像
    QVector<QString> friends;   //好友，存储mainwindow中_people的索引
    QString account;        //账号
};

#endif // PEOPLE_H

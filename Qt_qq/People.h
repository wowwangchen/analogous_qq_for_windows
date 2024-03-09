#ifndef PEOPLE_H
#define PEOPLE_H

#include<QIcon>
class People
{
public:
    People();
    People(QString);

    QIcon photo;            //头像
    QString name;             //姓名
    QVector<int> friends;   //好友，存储mainwindow中_people的索引
    QString account;        //账号
};

#endif // PEOPLE_H

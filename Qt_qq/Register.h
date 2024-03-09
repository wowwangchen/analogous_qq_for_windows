#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include<QMouseEvent>
#include<QMovie>
#include<QMessageBox>
#include<QGraphicsBlurEffect>
#include<QIcon>
#include<QPainter>
#include"moreSlection.h"
#include<qDebug>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QCryptographicHash>
#include"ChatMainWindow.h"
#include"registerUp.h"
#include"People.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QWidget
{
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr);
    ~Register();
    void setAllStyleSheet();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void checkCanRegister();
    QPixmap getCirclePhoto(QPixmap &src,QPixmap &mask,QSize maskSize);
    void initDataBase();                                     //初始化数据库
    void judgeCanLogin(QString account,QString password);    //判断是否可以登陆
    QString encryptData(QString s);                          //密码加密
    void LogInSuccess();                                     //登录成功
    void LogInFail();                                        //登录失败
    void initPeople();                                       //从数据库中初始化系统中的所有人的信息
public slots:
    void setLogUp();
    void setChangePassword();
    void AddnewAccount(QString account,QString password);
    void changeAccount(QString account,QString Oldpassword,QString Newpassword);
private slots:

    void on_accountLineEdit_textChanged(const QString &arg1);

    void on_passwordLineEdit_textChanged(const QString &arg1);

    void on_agreeCheckBox_stateChanged(int arg1);

    void on_closeButton_clicked();

    void on_registerButton_clicked();

    void on_onTopButton_clicked();


    void on_moreSelectionButton_clicked();

private:
    Ui::Register *ui;
    QPoint m_lastPos;                 //鼠标重载时间需要记录上次的点
    QMovie* backgroundMovie;          //背景的动画
    QGraphicsBlurEffect* blurEffect;  //背景的模糊
    bool onTop;
    moreSlection* moreSlectionDialog;//更多选项
    registerUp *up;                  //更多选项派生出的窗口登录

    QSqlDatabase qq_account;            //数据库
    ChatMainWindow *chatMainwindow;     //登录成功后的主窗口
    QVector<People> _people;
};
#endif // REGISTER_H

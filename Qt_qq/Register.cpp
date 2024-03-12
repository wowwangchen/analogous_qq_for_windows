#include "Register.h"
#include "./ui_Register.h"

Register::Register(QWidget *parent): QWidget(parent), ui(new Ui::Register)
{
    ui->setupUi(this);
    onTop=false;
    setAllStyleSheet();
    initDataBase();
    initPeople();

    connect(moreSlectionDialog,&moreSlection::logUp,this,&Register::setLogUp);
    connect(moreSlectionDialog,&moreSlection::changePassword,this,&Register::setChangePassword);
    connect(up,&registerUp::registerUpOver,this,&Register::AddnewAccount);
    connect(up,&registerUp::changePasswordOver,this,&Register::changeAccount);
}

Register::~Register()
{
    delete backgroundMovie;
    delete blurEffect;
    moreSlectionDialog->deleteLater();
    up->deleteLater();
    chatMainwindow->deleteLater();
    delete ui;
}

void Register::setAllStyleSheet()
{
    //程序图片
    QIcon iconIcon(":/picture/icon.jpg");
    setWindowIcon(iconIcon);
    //去除窗体边框
    setWindowFlags(Qt::FramelessWindowHint);

    //标题
    ui->photoLabel->setFixedSize(100, 100);
    QPixmap titlePixmap(":/picture/title.png");
    ui->titleLabel->setPixmap(titlePixmap);

    //背景
    blurEffect = new QGraphicsBlurEffect();  //模糊半径
    blurEffect->setBlurRadius(10); // 设置模糊半径，值越大越模糊
    ui->backgroundLabel->setGraphicsEffect(blurEffect);

    backgroundMovie=new QMovie(":/picture/background.gif");       //为label添加动画
    backgroundMovie->setScaledSize(ui->backgroundLabel->size());
    ui->backgroundLabel->setMovie(backgroundMovie);
    backgroundMovie->start();

    //头像
    ui->photoLabel->setFixedSize(100, 100);
    QPixmap photoPixmap(":/picture/photo.png");
    ui->photoLabel->setPixmap(photoPixmap);
    photoPixmap.scaled(photoPixmap.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);


    //账号密码(qss中有圆角代码)
    ui->accountLineEdit->setPlaceholderText("输入QQ账号"); //提示文本
        ui->accountLineEdit->setAlignment(Qt::AlignCenter);   //居中
    ui->passwordLineEdit->setPlaceholderText("输入QQ密码");
    ui->passwordLineEdit->setAlignment(Qt::AlignCenter);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    //登录按钮
    ui->registerButton->setEnabled(false);

    //置顶按钮
    QIcon onTopIcon(":/picture/ontheTop.png");
    ui->onTopButton->setIcon(onTopIcon);
    ui->onTopButton->setIconSize(ui->onTopButton->size());


    //更多选项
    ui->moreSelectionButton->setCursor(Qt::PointingHandCursor);
    moreSlectionDialog=new moreSlection(this);
    moreSlectionDialog->hide();

    //创建账号或更新密码
    up=new registerUp();
    up->hide();

    //登录界面
    chatMainwindow=new ChatMainWindow;
    this->hide();
}

void Register::checkCanRegister()
{
    if(ui->accountLineEdit->text()!=""&&ui->passwordLineEdit->text()!=""&&ui->agreeCheckBox->isChecked())
    {
        ui->registerButton->setStyleSheet("QPushButton {"
                                          "font: 14pt \"Segoe Print\";"
                                          "background-color: rgb(0,153,255);"
                                          "color: rgb(255, 255, 255);"
                                          "border-radius: 10px;"
                                          "padding: 10px;"
                                          "}"
                                          "QPushButton:pressed {"
                                          "background-color: rgb(0, 129, 214);"
                                          "color: rgb(63, 160, 224);"
                                          "}");
        ui->registerButton->setEnabled(true);
    }
    else
    {
        ui->registerButton->setStyleSheet("QPushButton {"
                                          "font: 14pt \"Segoe Print\";"
                                          "background-color: rgb(156, 216, 255);"
                                          "color: rgb(255, 255, 255);"
                                          "border-radius: 10px;"
                                          "padding: 10px;"
                                          "}"
                                          "QPushButton:pressed {"
                                          "background-color: rgb(0, 129, 214);"
                                          "color: rgb(63, 160, 224);"
                                          "}");
        ui->registerButton->setEnabled(false);
    }
}

QPixmap Register::getCirclePhoto(QPixmap &src, QPixmap &mask, QSize maskSize)
{
    if(maskSize==QSize(0,0))
        maskSize=mask.size();
    else
    {
        mask.scaled(maskSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    QImage resultImage(maskSize,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(),Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0,0,mask);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(0,0,src.scaled(Qt::KeepAspectRatio,Qt::SmoothTransformation));
    painter.end();

    return QPixmap::fromImage(resultImage);

}

void Register::initDataBase()
{
    qq_account=QSqlDatabase::addDatabase("QMYSQL");
    qq_account.setHostName("localhost");//本地主机IP和"127.0.0.1"一样
    qq_account.setPort(3306);//默认端口
    qq_account.setDatabaseName("qq_account");//连接的数据库
    qq_account.setUserName("root");//用户
    qq_account.setPassword("wclove0812.");//数据库密码
    //注意:需要在MySQL中先创建一个数据库才可以

    if (qq_account.open())
    {
        qDebug()<<"connect Success!!!";
    }
    qq_account.close();
}

void Register::judgeCanLogin(QString account, QString password)
{
    //注意单引号
    QString selectQuery = "SELECT * FROM account_password WHERE account = '"+account+"' AND password = '"+password+"'";

    qq_account.open();
    QSqlQuery query;            //用于执行命令
    if (query.exec(selectQuery))
    {
        if (query.next())
        {
            //LogInSuccess();
            this->hide();

            chatMainwindow->connectToServer(ui->accountLineEdit->text());
            qDebug()<<ui->accountLineEdit->text()+"ui->accountLineEdit->text()";

            QVector<QString> friends;
            friends.push_back("zhoumi");
            friends.push_back("21344");
            friends.push_back("1");
            chatMainwindow->initMyself(ui->accountLineEdit->text(),friends);
            chatMainwindow->show();
            connect(chatMainwindow,&ChatMainWindow::exitWindow,this,[=](){
                //析构
            });  //关闭聊天界面后，登录页面内存也要释放
        }
        else
        {
            LogInFail();
        }
    }
    else
    {
        qDebug() << "查询执行失败:" << query.lastError().text();
    }
    qq_account.close();
}

QString Register::encryptData(QString s)
{
    QByteArray dataBytes = s.toUtf8();
    QByteArray encryptedBytes = QCryptographicHash::hash(dataBytes, QCryptographicHash::Sha256);
    QString encryptedData = QString(encryptedBytes.toHex());
    return encryptedData;
}

void Register::LogInSuccess()
{
    // 数据库中存在匹配的账号和密码
    QMessageBox::information(this,"提示:","登陆成功");
}

void Register::LogInFail()
{
    // 数据库中不存在匹配的账号和密码
    QMessageBox::warning(this,"提示:","登录失败");
    ui->passwordLineEdit->clear();
}

void Register::initPeople()
{
    if (qq_account.open()) {
        // 执行查询语句
        QSqlQuery query("SELECT account FROM account_password");

        // 遍历查询结果
        while (query.next()) {
            QString account = query.value(0).toString();

            // 创建People对象并添加到容器中
            People aPeople(account);
            _people.append(aPeople);
            qDebug()<<"add success"+account;
        }

        qq_account.close();
    } else {
        // 处理数据库连接错误
        qDebug() << "Failed to connect to MySQL database";
    }
}

QString Register::getLocalIP()
{
    QString IP="";

    QString localname=QHostInfo::localHostName();
    QHostInfo hostinfo=QHostInfo::fromName(localname);
    QList<QHostAddress> list=hostinfo.addresses();
    if(!list.isEmpty())
    {
        for(int i=0;i<list.count();i++)
        {
            if(list.at(i).protocol()==QAbstractSocket::IPv4Protocol)
            {
                IP=list.at(i).toString();
                break;
            }
        }
    }
    return IP;
}

void Register::getAllIP()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // 遍历每个网络接口
    for (const QNetworkInterface& interface : interfaces)
    {
        qDebug() << "设备名称:" << interface.humanReadableName();
                                         qDebug() << "硬件地址:" << interface.hardwareAddress();

        // 遍历每个接口的IP地址
        QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
        for (const QNetworkAddressEntry& entry : addressEntries)
        {
            qDebug() << "IP地址:" << entry.ip().toString();
            qDebug() << "子网掩码:" << entry.netmask().toString();
            qDebug() << "广播地址:" << entry.broadcast().toString();
        }

        qDebug() << "-----------------";
    }
}


void Register::setLogUp()
{
    up->changeModeToLogUp();
    up->show();
}

void Register::setChangePassword()
{
    up->changeModeToChangePassword();
    up->show();
}

void Register::AddnewAccount(QString account, QString password)
{
    //注意单引号
    QString insertQuery = "insert into account_password values('"+account+"','"+password+"')";
    qq_account.open();
    QSqlQuery query;            //用于执行命令

    query.prepare(insertQuery);
    query.bindValue(":account", account);
    query.bindValue(":password", password);

    if (query.exec())
    {
        QMessageBox::information(this, "提示", "添加成功");
        _people.append(People(account));
    }
    else
    {
        QMessageBox::information(this, "提示", "添加失败: " + query.lastError().text());
    }

    qq_account.close();
}

void Register::changeAccount(QString account, QString Oldpassword, QString Newpassword)
{
    //注意单引号
    QString selectQuery = "SELECT * FROM account_password WHERE account = '"+account+"' AND password = '"+Oldpassword+"'";
    qq_account.open();
    QSqlQuery query;            //用于执行命令
    if (query.exec(selectQuery))
    {
        //如果旧密码正确
        if (query.next())
        {
            selectQuery = "update account_password set password = '"+Newpassword+"' where account ='"+account+"'";
            QSqlQuery updateQuery;
            updateQuery.prepare(selectQuery);
            updateQuery.bindValue(":newPassword", Newpassword);
            updateQuery.bindValue(":account", account);

            if (updateQuery.exec())
            {
                QMessageBox::information(this, "提示", "密码修改成功");
            }
            else
            {
                QMessageBox::information(this, "提示", "密码修改失败: " + updateQuery.lastError().text());
            }
        }
        else
        {
            QMessageBox::information(this,"提示","error");
        }
    }
    else
    {
        qDebug() << "查询执行失败:" << query.lastError().text();
    }




    qq_account.close();
}



//以下为鼠标重载函数
void Register::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        // 记录鼠标按下时的位置
        m_lastPos = event->globalPos();
    }

    //只要点击了就隐藏，点其他地方直接隐藏，点按钮，隐藏后进入选择的内容
    moreSlectionDialog->hide();
}

void Register::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // 计算鼠标移动的距离
        QPoint delta = event->globalPos() - m_lastPos;
        // 移动窗口
        move(pos() + delta);
        // 更新鼠标按下时的位置
        m_lastPos = event->globalPos();
    }
}


//以下为槽函数
void Register::on_accountLineEdit_textChanged(const QString &arg1)
{
    checkCanRegister();
}


void Register::on_passwordLineEdit_textChanged(const QString &arg1)
{
    checkCanRegister();
}


void Register::on_agreeCheckBox_stateChanged(int arg1)
{
    checkCanRegister();
}


void Register::on_closeButton_clicked()
{
    this->close();
}


void Register::on_registerButton_clicked()
{
    QString s1=ui->accountLineEdit->text();
    QString s2=ui->passwordLineEdit->text();
    //s2=encryptData(s2);
    judgeCanLogin(s1,s2);
}


void Register::on_onTopButton_clicked()
{
    onTop=!onTop;
    if(onTop==true)
    {
        setWindowFlags(Qt::WindowStaysOnTopHint);
    }
    else
    {
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowStaysOnTopHint; // 移除置顶标志位
        setWindowFlags(flags);
    }
}


void Register::on_moreSelectionButton_clicked()
{
    QPoint point=QPoint(ui->moreSelectionButton->x()+80,ui->moreSelectionButton->y()-55);
    moreSlectionDialog->move(point);
    moreSlectionDialog->show();

}


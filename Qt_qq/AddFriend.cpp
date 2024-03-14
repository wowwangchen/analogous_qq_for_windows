#include "AddFriend.h"
#include "ui_AddFriend.h"

AddFriend::AddFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);

    initBack();
}

AddFriend::~AddFriend()
{
    delete backgroundMovie;
    delete ui;
}

QString AddFriend::getAccount()
{
    return ui->lineEdit->text();
}

void AddFriend::on_addButton_clicked()
{
    emit addFriend(ui->lineEdit->text());
}

void AddFriend::initBack()
{
    backgroundMovie=new QMovie(":/picture/scenery.gif");       //为label添加动画
    backgroundMovie->setScaledSize(ui->backLabel->size());
    ui->backLabel->setMovie(backgroundMovie);
    backgroundMovie->start();
}


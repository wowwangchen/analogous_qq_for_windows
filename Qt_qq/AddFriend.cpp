#include "AddFriend.h"
#include "ui_AddFriend.h"

AddFriend::AddFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);

    //模态对话框，打开时无法对其他窗体进行交互
    //将会阻塞应用程序的所有其他窗口，包括主窗口和其他对话框窗口
    setWindowModality(Qt::ApplicationModal);

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
    if(!ui->lineEdit->text().isEmpty())
        emit addFriend(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void AddFriend::initBack()
{
    backgroundMovie=new QMovie(":/picture/scenery2.gif");       //为label添加动画
    backgroundMovie->setScaledSize(ui->backLabel->size());
    ui->backLabel->setMovie(backgroundMovie);
    backgroundMovie->start();


}


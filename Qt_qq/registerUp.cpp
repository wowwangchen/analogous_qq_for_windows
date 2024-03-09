#include "registerUp.h"
#include "ui_registerUp.h"

registerUp::registerUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerUp)
{
    ui->setupUi(this);
}

registerUp::~registerUp()
{
    delete ui;
}

void registerUp::changeModeToChangePassword()
{
    ui->enterPasswordLabel->setText("enter the old password");
    ui->enterPassword2Label->setText("enter the new password");
}

void registerUp::changeModeToLogUp()
{
    ui->enterPasswordLabel->setText("enter password");
    ui->enterPassword2Label->setText("confirm password");
}


void registerUp::on_enterButton_clicked()
{
    QString s1=ui->accountEdit->text();
    QString s2=ui->lineEdit_2->text();
    QString s3=ui->lineEdit_3->text();

    if(ui->enterPasswordLabel->text()=="enter password"&&s2!=s3)
    {
        QMessageBox::information(this,"Message","The two passwords are different");
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        return;
    }
    if(ui->enterPasswordLabel->text()=="enter password")
    {
        emit registerUpOver(s1,s2);
        this->close();
    }
    else if(ui->enterPasswordLabel->text()=="enter the old password")
    {
        emit changePasswordOver(s1,s2,s3);
        this->close();
    }
    ui->accountEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
}


void registerUp::on_cancelButton_clicked()
{
    ui->accountEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    this->close();
}


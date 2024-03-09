#include "moreSlection.h"
#include "ui_moreSlection.h"

moreSlection::moreSlection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moreSlection)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

moreSlection::~moreSlection()
{
    delete ui;
}


void moreSlection::on_pushButton_clicked()
{
    emit logUp();
}


void moreSlection::on_pushButton_2_clicked()
{
    emit changePassword();
}


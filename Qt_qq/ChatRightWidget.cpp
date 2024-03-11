#include "ChatRightWidget.h"
#include "ui_ChatRightWidget.h"

ChatRightWidget::ChatRightWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRightWidget)
{
    ui->setupUi(this);
}

ChatRightWidget::~ChatRightWidget()
{
    delete ui;
}

void ChatRightWidget::setMessage(QString str)
{
    ui->messageLabel->setText(str);
}

void ChatRightWidget::setSize(int n)
{
    ui->messageLabel->setFixedSize(QSize(20+n*30,60));
}

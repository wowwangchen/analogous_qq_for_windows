#include "ChatLeftWidget.h"
#include "ui_ChatLeftWidget.h"

ChatLeftWidget::ChatLeftWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatLeftWidget)
{
    ui->setupUi(this);
}

ChatLeftWidget::~ChatLeftWidget()
{
    delete ui;
}

void ChatLeftWidget::setMessage(QString str)
{
    ui->MessageLabel->setText(str);
}

void ChatLeftWidget::setSize(int n)
{
    ui->MessageLabel->setFixedSize(QSize(20+n*30,60));
}


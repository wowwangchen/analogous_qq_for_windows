#include "theFriendMessage.h"
#include "ui_theFriendMessage.h"

theFriendMessage::theFriendMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::theFriendMessage)
{
    ui->setupUi(this);
}

theFriendMessage::~theFriendMessage()
{
    delete ui;
}

void theFriendMessage::setName(QString name)
{
    ui->nameLabel->setText(name);
}

QString theFriendMessage::getName()
{
    return ui->nameLabel->text();
}

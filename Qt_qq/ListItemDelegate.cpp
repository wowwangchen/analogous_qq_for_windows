#include "ListItemDelegate.h"

ListItemDelegate::ListItemDelegate(QObject* parent)
    : QStyledItemDelegate { parent }
{
}


void ListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString message = index.data(Qt::DisplayRole).toString();
    QString styleSheet = "font: 22pt \"Segoe Print\";"
                         "font-size: 22px;"
                         "border-radius: 10px;"
                         "padding: 10px;";

    QWidget *widget = const_cast<QWidget*>(option.widget);
    widget->setStyleSheet(styleSheet);


    if(message.size()>=3 &&message[0]=="a"&&message[1]=="1"&&message[2]=="`")
    {
        qDebug()<<"left";
        message.remove(0,3);
        // 收到的消息，靠左对齐，使用不同的颜色和背景
        painter->setPen(Qt::blue);
        painter->setBrush(QColor(220, 220, 255));
        painter->drawRect(option.rect.adjusted(2, 2, -2, -2));
        painter->drawText(option.rect.adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignVCenter, message);
    }
    else
    {
        qDebug()<<"right";
        // 发送的消息，靠右对齐，使用不同的颜色和背景
        painter->setPen(Qt::darkGreen);
        painter->setBrush(QColor(220, 255, 220));
        painter->drawRect(option.rect.adjusted(2, 2, -2, -2));
        painter->drawText(option.rect.adjusted(5, 5, -5, -5), Qt::AlignRight | Qt::AlignVCenter, message);
    }
}


void ListItemDelegate::setMessage(QString mes)
{
    messageTexts.push_back(mes);
}

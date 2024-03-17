#include "ListItemDelegate.h"

ListItemDelegate::ListItemDelegate(QObject* parent)
    : QStyledItemDelegate { parent }
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
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

    int fontSize = 14;
    QFont font = painter->font();
    font.setPointSize(fontSize);
    painter->setFont(font);

    //调整每个框的大小
    QRect adjustedRect = option.rect.adjusted(2, 2, -2, -2);
    adjustedRect.setHeight(adjustedRect.height() + 20);



    //左对齐
    if(message.size()>=3 &&message.startsWith("```"))
    {
        qDebug()<<"delegate:"<<message;
        message.remove(0,3);
        // 收到的消息，靠左对齐，使用不同的颜色和背景
        painter->setPen(Qt::blue);
        painter->setBrush(QColor(220, 220, 255));
        painter->drawRect(adjustedRect.adjusted(2, 2, -2, -2));
        painter->drawText(adjustedRect.adjusted(5, 5, -5, -5), Qt::AlignLeft | Qt::AlignVCenter, message);
    }


    //发送文件
    else if(message.startsWith("{{{}}}"))
    {
        qDebug()<<"11111111111";
        // 删除"{{{}}}"字符
        message.remove("{{{}}}");

        // 添加前缀
        message = "打开文件:" + message;

        painter->setPen(Qt::blue);
        painter->setBrush(QColor(220, 220, 255));
        painter->drawRect(adjustedRect.adjusted(2, 2, -2, -2));

        QStyledItemDelegate::paint(painter, option, index);

        // 绘制按钮
        if (option.state & QStyle::State_Selected)
        {
            QStyleOptionButton buttonOption;
            //buttonOption.text = QString::fromUtf8(message.toUtf8());
            buttonOption.rect = option.rect.adjusted(option.rect.width() - 60, 0, -10, 0); // 按钮的位置和大小
            buttonOption.state |= QStyle::State_Enabled;
            QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);

        }

    }


    //右对齐
    else
    {
        // 发送的消息，靠右对齐，使用不同的颜色和背景
        painter->setPen(Qt::darkGreen);
        painter->setBrush(QColor(220, 255, 220));
        painter->drawRect(adjustedRect.adjusted(2, 2, -2, -2));
        painter->drawText(adjustedRect.adjusted(5, 5, -5, -5), Qt::AlignRight | Qt::AlignVCenter, message);
    }
}


void ListItemDelegate::setMessage(QString mes)
{
    messageTexts.push_back(mes);
}

#include "ListItemDelegateLeft.h"

ListItemDelegateLeft::ListItemDelegateLeft(QObject *parent)
{

}

void ListItemDelegateLeft::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void ListItemDelegateLeft::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //auto opt { option };
    //opt.displayAlignment = Qt::AlignRight;
    QStyledItemDelegate::paint(painter, option, index);

    // 获取列表项的矩形区域
    QRect rect = option.rect;



    // 创建头像Label部件
    QLabel *avatarLabel = new QLabel();
    avatarLabel->setFixedSize(50, 50); // 设置头像大小
    avatarLabel->setStyleSheet("image: url(:/picture/photo.png);"); // 设置头像背景颜色，仅作为示例


    // 创建消息Label部件
    QLabel *messageLabel = new QLabel();
    messageLabel->setFixedSize(40+messageTexts.value(index.row()).size()*20, 60); // 设置消息展示大小
    messageLabel->setStyleSheet("background-color: rgb(239, 255, 249);"
                                "font: 12pt \"Segoe Print\";"
                                "border-radius: 20px;"
                                "padding: 10px;");
    messageLabel->setText(messageTexts.value(index.row()));
    qDebug()<<messageTexts;

    // 创建水平布局，并将消息Label部件添加到布局中
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(messageLabel);
    layout->addWidget(avatarLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 设置布局的对齐方式为右对齐
    layout->setAlignment(Qt::AlignLeft);

    // 创建一个临时的QWidget部件，并将布局应用到该部件上
    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    // 计算部件的位置
    int widgetWidth = widget->sizeHint().width();
    //int x = rect.x() + rect.width() - widgetWidth;
    int x = rect.x()+10;
    int y = rect.y() + (rect.height() - widget->sizeHint().height()) / 2;



    // 绘制部件
    painter->save();
    painter->translate(x, y);
    widget->render(painter);
    painter->restore();
}

void ListItemDelegateLeft::setMessage(QString mes)
{
        messageTexts.push_back(mes);
}

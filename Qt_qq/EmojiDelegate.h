#ifndef EMOJIDELEGATE_H
#define EMOJIDELEGATE_H

#include<QStyledItemDelegate>
#include<QPainter>
class EmojiDelegate: public QStyledItemDelegate
{
public:
    EmojiDelegate();

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        // 设置项的大小
        return QSize(50, 50); // 根据需要调整项的宽度和高度
    }
//    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
//    {
////        // 获取项的数据
////        QString text = index.data(Qt::DisplayRole).toString();

////        // 绘制项的背景
////        QStyledItemDelegate::paint(painter, option, index);

////        // 放大项中的内容
////        QFont originalFont = painter->font();
////        QFont scaledFont = originalFont;
////        if (originalFont.pointSize() > 0)
////            scaledFont.setPointSize(originalFont.pointSize() * 2); // 将字体大小放大两倍
////        else
////           scaledFont.setPointSize(10); // 将字体大小放大两倍
////        painter->setFont(scaledFont);

////        // 绘制项的内容
////        painter->drawText(option.rect, Qt::AlignCenter, text);
//    }
};

#endif // EMOJIDELEGATE_H

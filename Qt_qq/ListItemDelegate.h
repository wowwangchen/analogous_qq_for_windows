#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include<QObject>
#include<QStyledItemDelegate>
#include<qDebug>
#include<QPushButton>
#include<QLabel>
#include<QHBoxLayout>
#include<QPainter>
#include<QVector>
#include<QString>
#include<QList>
#include<QPixmap>
#include<QRect>
#include<QStyleOptionViewItem>
#include<QApplication>
class ListItemDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ListItemDelegate(QObject* parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
                                        const override;

public slots:
    void setMessage(QString mes);

private:
    QList<QString> messageTexts;



};

#endif // LISTITEMDELEGATE_H

#ifndef LISTITEMDELEGATELEFT_H
#define LISTITEMDELEGATELEFT_H


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
class ListItemDelegateLeft:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ListItemDelegateLeft(QObject* parent = nullptr);
    //QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,
                              const QModelIndex &index) const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
        const override;

public slots:
    void setMessage(QString mes);

private:
    QVector<QLabel*> _labels;
    QVector<QHBoxLayout*> _layouts;
    QVector<QWidget*> _widgets;

    QList<QString> messageTexts;

};

#endif // LISTITEMDELEGATELEFT_H

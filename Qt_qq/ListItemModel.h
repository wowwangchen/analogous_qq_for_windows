#ifndef LISTITEMMODEL_H
#define LISTITEMMODEL_H


#include<QObject>
#include<QAbstractListModel>
class ListItemModel:public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListItemModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setData(const QStringList &data, const QStringList &data2);

private:
    QStringList m_data;
    QStringList m_data2;
};

#endif // LISTITEMMODEL_H

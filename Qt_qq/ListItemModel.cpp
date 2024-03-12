#include "ListItemModel.h"



ListItemModel::ListItemModel(QObject *parent)
{

}

int ListItemModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent);
    return m_data.size();

}

QVariant ListItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_data.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::UserRole)
        return m_data.at(index.row());

    if (role == Qt::UserRole + 1)
        return m_data2.at(index.row());

    return QVariant();


}

void ListItemModel::setData(const QStringList &data, const QStringList &data2)
{

    beginResetModel();
    m_data = data;
    m_data2 = data2;
    endResetModel();
}

#ifndef EMOJIMODEL_H
#define EMOJIMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class EmojiModel:public QAbstractListModel
{
public:
    //构造函数
    explicit EmojiModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        generateEmojiList();
    }

    //返回有多少行
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return m_emojiList.count();
    }

    //返回索引指向的数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() >= m_emojiList.count())
            return QVariant();

        if (role == Qt::DisplayRole || role == Qt::EditRole)
            return m_emojiList.at(index.row());

        return QVariant();
    }

private:
    //初始化表情数据
    void generateEmojiList()
    {
        char32_t t[] = {0x1F601, 0x0}; // 加上0x00是为了防止表情后面跟随乱码
        for (char32_t i = 0x1F601; i <= 0x1F64F; ++i)
        {
            t[0] = i;
            QString emoji = QString::fromUcs4(t);
            m_emojiList.append(emoji);
        }
    }

    QStringList m_emojiList;
};

#endif // EMOJIMODEL_H

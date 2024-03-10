#ifndef THEFRIENDMESSAGE_H
#define THEFRIENDMESSAGE_H

#include <QWidget>

namespace Ui {
class theFriendMessage;
}

class theFriendMessage : public QWidget
{
    Q_OBJECT

public:
    explicit theFriendMessage(QWidget *parent = nullptr);
    ~theFriendMessage();
    void setName(QString);
    QString getName();

private:
    Ui::theFriendMessage *ui;
    bool isPeople;  //yes 就是好友，no就是群聊
};

#endif // THEFRIENDMESSAGE_H

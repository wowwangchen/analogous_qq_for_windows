#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include<QMovie>
namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriend(QWidget *parent = nullptr);
    ~AddFriend();
    QString getAccount();
    void initBack();
signals:
    void addFriend(QString);

private slots:
    void on_addButton_clicked();


private:
    Ui::AddFriend *ui;
    QMovie* backgroundMovie;
};

#endif // ADDFRIEND_H

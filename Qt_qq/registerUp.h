#ifndef REGISTERUP_H
#define REGISTERUP_H

#include <QWidget>
#include<QMessageBox>
namespace Ui {
class registerUp;
}

class registerUp : public QWidget
{
    Q_OBJECT

public:
    explicit registerUp(QWidget *parent = nullptr);
    ~registerUp();
    void changeModeToChangePassword();
    void changeModeToLogUp();

private slots:
    void on_enterButton_clicked();
    void on_cancelButton_clicked();

signals:
    void registerUpOver(QString account,QString password);
    void changePasswordOver(QString account,QString Oldpassword,QString Newpassword);

private:
    Ui::registerUp *ui;
};

#endif // REGISTERUP_H

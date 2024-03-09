#ifndef MORESLECTION_H
#define MORESLECTION_H

#include <QWidget>
#include<QMouseEvent>

namespace Ui {
class moreSlection;
}

class moreSlection : public QWidget
{
    Q_OBJECT

public:
    explicit moreSlection(QWidget *parent = nullptr);
    ~moreSlection();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
signals:
    void logUp();
    void changePassword();

private:
    Ui::moreSlection *ui;
};

#endif // MORESLECTION_H

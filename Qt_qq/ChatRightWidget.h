#ifndef CHATRIGHTWIDGET_H
#define CHATRIGHTWIDGET_H

#include <QWidget>

namespace Ui {
class ChatRightWidget;
}

class ChatRightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRightWidget(QWidget *parent = nullptr);
    ~ChatRightWidget();
    void setMessage(QString str);
    void setSize(int n);

private:
    Ui::ChatRightWidget *ui;
};

#endif // CHATRIGHTWIDGET_H

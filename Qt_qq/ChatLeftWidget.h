#ifndef CHATLEFTWIDGET_H
#define CHATLEFTWIDGET_H

#include <QWidget>

namespace Ui {
class ChatLeftWidget;
}

class ChatLeftWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatLeftWidget(QWidget *parent = nullptr);
    ~ChatLeftWidget();
    void setMessage(QString str);
    void setSize(int n);

private:
    Ui::ChatLeftWidget *ui;
};

#endif // CHATLEFTWIDGET_H

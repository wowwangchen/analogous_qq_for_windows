#include "Register.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置Qt应用程序的风格为Fusion
//    app.setStyle("// 设置Qt应用程序的风格为Fusion");
//    // 获取当前应用程序的样式表
//    QString styleSheet = app.styleSheet();
//    // 在样式表中添加自定义规则，将顶部导航栏的背景色设置为白色
//    styleSheet += "QHeaderView::section { background-color: white; }";
//    // 设置修改后的样式表
//    app.setStyleSheet(styleSheet);


    //Register w;
    Register w;
    w.show();
    return app.exec();
}

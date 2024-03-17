#include "Register.h"

#include <QApplication>
#include<QTextCodec>
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



//设置中文编码
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#endif



    //Register w;
    Register w;
    w.show();
    return app.exec();
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序的图标
    app.setWindowIcon(QIcon(":/assets/icons/logo.jpg"));

    MainWindow window; 
    window.show();
    return app.exec();
}

#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    while(QDialog::Accepted==l.exec())
    {
        MainWindow w;
        w.show();
        a.exec();
    }
    return 0;
}

#include "mainwindow.h"
#include "TCP-client/FileHandeling.h"
#include "TCP-client/tcpHandler.h"

#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}

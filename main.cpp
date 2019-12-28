#include <QApplication>

#include "mainwindow.h"
#include "dataframe3d.h"
#include "dataviewwidget3d.h"
#include "tinyply.h"
#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(window.sizeHint());
    window.show();

    return app.exec();
}

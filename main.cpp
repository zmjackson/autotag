#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"
#include "datatimeline.h"
#include "dataframe3d.h"
#include "dataviewwidget3d.h"
#include "tinyply.h"
#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{    
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication app(argc, argv);
    MainWindow window;
    window.resize(window.sizeHint());
    window.show();

    return app.exec();
}

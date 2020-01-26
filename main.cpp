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
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{    
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication app(argc, argv);
    QFile file("dark.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    app.setStyleSheet(stream.readAll());
    MainWindow window;
    window.resize(window.sizeHint());
    window.show();

    return app.exec();
}

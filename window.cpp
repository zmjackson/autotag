#include "window.h"
#include "mainwindow.h"
#include "dataviewwidget3d.h"

#include <QVBoxLayout>

Window::Window(MainWindow *parent) : mainWindow(parent)
{
    dataViewWidget = new DataViewWidget3D(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dataViewWidget);
    setLayout(mainLayout);

    setWindowTitle("autotag");
}

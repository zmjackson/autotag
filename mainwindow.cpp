#include "mainwindow.h"
#include "dataframe3d.h"
#include "dataviewwidget3d.h"
#include "controlwidget.h"
#include <string>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QSizePolicy>

MainWindow::MainWindow()
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *fileMenuWindow = menuBar->addMenu(tr("&File"));
    QAction *openFiles = new QAction(fileMenuWindow);
    openFiles->setText(tr("Open"));
    fileMenuWindow->addAction(openFiles);
    connect(openFiles, &QAction::triggered, this, &MainWindow::openFiles);
    setMenuBar(menuBar);

    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    dataViewWidget = new DataViewWidget3D(this);
    QSizePolicy viewerPolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    viewerPolicy.setVerticalStretch(10);
    dataViewWidget->setSizePolicy(viewerPolicy);
    connect(this, &MainWindow::currentDataFrameChanged,
            dataViewWidget, &DataViewWidget3D::changeCurrentDataFrame);
    connect(this, &MainWindow::newMaxDataPoints,
            dataViewWidget, &DataViewWidget3D::setMaxDataPoints);

    controlWidget = new ControlWidget(this);
    QSizePolicy controllerPolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    controllerPolicy.setVerticalStretch(1);
    controlWidget->setSizePolicy(controllerPolicy);
    connect(controlWidget, &ControlWidget::nextDataFrameRequest,
            this, &MainWindow::nextDataFrame);
    connect(controlWidget, &ControlWidget::prevDataFrameRequest,
            this, &MainWindow::prevDataFrame);

    mainLayout->addWidget(dataViewWidget);
    mainLayout->addWidget(controlWidget);

    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
}

void MainWindow::setCurrentDataFrame(const int timeStamp)
{
    timeline.setCurrentFrame(timeStamp);
    emit currentDataFrameChanged(timeline.currentFrame());
}

void MainWindow::nextDataFrame()
{
    std::cout << "MainWindow: next" << std::endl;
    timeline.nextFrame();
    emit currentDataFrameChanged(timeline.currentFrame());
}

void MainWindow::prevDataFrame()
{
    std::cout << "MainWindow: prev" << std::endl;
    timeline.prevFrame();
    emit currentDataFrameChanged(timeline.currentFrame());
}

void MainWindow::openFiles()
{
    QFileDialog openFilesDialog(this);
    openFilesDialog.setFileMode(QFileDialog::ExistingFiles);
    openFilesDialog.setNameFilter(tr("PLY files (*.ply)"));
    openFilesDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (openFilesDialog.exec())
        fileNames = openFilesDialog.selectedFiles();
    int oldMaxDataPoints = timeline.maxDataPoints();
    for (const QString &file : fileNames)
        timeline.addFrame({file});
    /*if (timeline.maxDataPoints() > oldMaxDataPoints)
        emit newMaxDataPoints(timeline.maxDataPoints());*/
}

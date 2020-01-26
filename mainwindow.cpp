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

    QAction *addTrackingData = new QAction(fileMenuWindow);
    addTrackingData->setText(tr("Add tracking data"));
    fileMenuWindow->addAction(addTrackingData);
    connect(addTrackingData, &QAction::triggered, this, &MainWindow::openTrackingDataFiles);

    setMenuBar(menuBar);

    QWidget *mainWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;

    dataViewWidget = new DataViewWidget3D(this);
    QSizePolicy viewerPolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    viewerPolicy.setHorizontalStretch(10);
    dataViewWidget->setSizePolicy(viewerPolicy);
    connect(this, &MainWindow::currentDataFrameChanged,
            dataViewWidget, &DataViewWidget3D::changeCurrentDataFrame);
    connect(this, &MainWindow::newMaxDataPoints,
            dataViewWidget, &DataViewWidget3D::setMaxDataPoints);

    controlWidget = new ControlWidget(this);
    QSizePolicy controllerPolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    controllerPolicy.setHorizontalStretch(1);
    controlWidget->setSizePolicy(controllerPolicy);
    connect(controlWidget, &ControlWidget::nextDataFrameRequest,
            this, &MainWindow::nextDataFrame);
    connect(controlWidget, &ControlWidget::prevDataFrameRequest,
            this, &MainWindow::prevDataFrame);
    connect(controlWidget, &ControlWidget::labelColorChanged,
            dataViewWidget, &DataViewWidget3D::addTrackingClass);

    mainLayout->addWidget(controlWidget);
    mainLayout->addWidget(dataViewWidget);

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
    timeline.nextFrame();
    emit currentDataFrameChanged(timeline.currentFrame());
}

void MainWindow::prevDataFrame()
{    
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
    DataFrame3D test;
    test.loadJson("test.json");
    /*if (timeline.maxDataPoints() > oldMaxDataPoints)
        emit newMaxDataPoints(timeline.maxDataPoints());*/
}

void MainWindow::openTrackingDataFiles()
{
    QFileDialog openFilesDialog(this);
    openFilesDialog.setFileMode(QFileDialog::ExistingFiles);
    openFilesDialog.setNameFilter(tr("Json files (*.json)"));
    openFilesDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (openFilesDialog.exec())
        fileNames = openFilesDialog.selectedFiles();
    timeline.addTrackingData(fileNames);
    for (const std::string &label : timeline.trackingLabels)
        controlWidget->addTrackingLabel(label);
}

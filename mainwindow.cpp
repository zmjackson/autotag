#include "mainwindow.h"
#include "window.h"

MainWindow::MainWindow()
{
    setCentralWidget(new Window(this));
}

void MainWindow::setCurrentDataFrame(const std::string &id)
{
    timeline.setCurrentFrame(id);
    emit currentDataFrameChanged(timeline.currentFrame());
}

void MainWindow::setCurrentDataFrame(int timeStamp)
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

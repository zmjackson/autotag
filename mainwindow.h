#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datatimeline.h"
#include <set>

class DataFrame3D;
class DataViewWidget3D;
class ControlWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();    

signals:
    void currentDataFrameChanged(const DataFrame3D &frame);
    void newMaxDataPoints(const int numDataPoints);

public slots:
    void setCurrentDataFrame(const int timeStamp);
    void nextDataFrame();
    void prevDataFrame();
    void openFiles();
    void openTrackingDataFiles();

private:
    DataTimeline timeline;
    DataViewWidget3D *dataViewWidget;
    ControlWidget *controlWidget;
    std::set<std::string> trackingLabels;
};

#endif // MAINWINDOW_H

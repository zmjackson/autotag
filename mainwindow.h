#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

#include <QMainWindow>

#include "datatimeline.h"
#include "dataframe3d.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void setCurrentDataFrame(int timeStamp);
    void setCurrentDataFrame(const std::string& id);
    void nextDataFrame();
    void prevDataFrame();

signals:
    void currentDataFrameChanged(const DataFrame3D &frame);

private:
    DataTimeline timeline;

};

#endif // MAINWINDOW_H

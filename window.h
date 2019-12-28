#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class DataViewWidget3D;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(MainWindow *parent);

private:
    DataViewWidget3D *dataViewWidget;
    MainWindow *mainWindow;
};
#endif // WINDOW_H

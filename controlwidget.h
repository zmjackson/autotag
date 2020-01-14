#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTimer;
QT_END_NAMESPACE

class MainWindow;

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    ControlWidget(MainWindow *parent);

signals:
    void nextDataFrameRequest();
    void prevDataFrameRequest();

public slots:
    void playPauseButtonPressed();

private:
    bool m_isPlaying;
    QPushButton *m_playPauseButton;
    QTimer *m_frameTimer;
    MainWindow *mainWindow;
    QPushButton *nextFrameButton;
    QPushButton *prevFrameButton;
};

#endif // CONTROLWIDGET_H

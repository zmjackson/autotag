#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QPixmap>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTimer;
class QListWidget;
class QString;
class QColor;
class QMouseEvent;
class QListWidgetItem;
QT_END_NAMESPACE

class MainWindow;

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    ControlWidget(MainWindow *parent);
    void addTrackingLabel(const std::string &label);

signals:
    void nextDataFrameRequest();
    void prevDataFrameRequest();
    void labelColorChanged(const std::string &label, const QColor &color);

public slots:
    void playPauseButtonPressed();
    void openColorDialog(QListWidgetItem *item);

private:
    QPixmap getColorSwatch(const QColor &color);

    bool m_isPlaying;
    QPushButton *m_playPauseButton;
    QTimer *m_frameTimer;
    MainWindow *mainWindow;
    QPushButton *nextFrameButton;
    QPushButton *prevFrameButton;
    QListWidget *trackingColors;
    QStringList colorNames;
    int numTrackingLabels;
};

#endif // CONTROLWIDGET_H

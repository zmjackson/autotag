#include "controlwidget.h"
#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
#include <QColor>
#include <QColorDialog>

ControlWidget::ControlWidget(MainWindow *parent)
    : mainWindow(parent),
      numTrackingLabels(0),
      colorNames(QColor::colorNames())
{    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_playPauseButton = new QPushButton(tr("Play"), this);
    mainLayout->addWidget(m_playPauseButton);
    connect(m_playPauseButton, &QPushButton::clicked,
            this, &ControlWidget::playPauseButtonPressed);

    QHBoxLayout *navigationButtonLayout = new QHBoxLayout;
    nextFrameButton = new QPushButton(tr("Next"), this);
    prevFrameButton = new QPushButton(tr("Previous"), this);
    navigationButtonLayout->addWidget(prevFrameButton);
    navigationButtonLayout->addWidget(nextFrameButton);
    connect(nextFrameButton, &QPushButton::clicked,
            this, &ControlWidget::nextDataFrameRequest);
    connect(prevFrameButton, &QPushButton::clicked,
            this, &ControlWidget::prevDataFrameRequest);

    QWidget *navigationContainer = new QWidget;
    navigationContainer->setLayout(navigationButtonLayout);
    mainLayout->addWidget(navigationContainer);

    QLabel *colorsListLabel = new QLabel(tr("Tracking Labels"), this);
    mainLayout->addWidget(colorsListLabel);

    trackingColors = new QListWidget(this);
    mainLayout->addWidget(trackingColors);

    setLayout(mainLayout);

    connect(trackingColors, &QListWidget::itemDoubleClicked,
            this, &ControlWidget::openColorDialog);

    m_frameTimer = new QTimer(this);
    connect(m_frameTimer, &QTimer::timeout,
            this, &ControlWidget::nextDataFrameRequest);
    m_frameTimer->setInterval(100); // 10 Hz lidar sensor
}

void ControlWidget::playPauseButtonPressed()
{
    if (!m_isPlaying) {
        m_isPlaying = true;
        m_playPauseButton->setText("Pause");
        m_frameTimer->start();
    }
    else {
        m_isPlaying = false;
        m_playPauseButton->setText("Play");
        m_frameTimer->stop();
    }
}

void ControlWidget::openColorDialog(QListWidgetItem *item)
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);
    item->setIcon(getColorSwatch(color));
    emit labelColorChanged(item->text().toStdString(), color);
}

void ControlWidget::addTrackingLabel(const std::string &label)
{
    QListWidgetItem *newLabel = new QListWidgetItem;
    newLabel->setText(QString(label.c_str()));
    QColor color(colorNames[numTrackingLabels++]);
    newLabel->setIcon(getColorSwatch(color));
    trackingColors->addItem(newLabel);
    emit labelColorChanged(label, color);
}

QPixmap ControlWidget::getColorSwatch(const QColor &color)
{
    QPixmap swatch(100, 100);
    swatch.fill(color);
    return swatch;
}

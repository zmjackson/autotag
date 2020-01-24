#include "controlwidget.h"
#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

ControlWidget::ControlWidget(MainWindow *parent) : mainWindow(parent)
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

    setLayout(mainLayout);

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

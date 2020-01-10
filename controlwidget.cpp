#include "controlwidget.h"
#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <iostream>

ControlWidget::ControlWidget(MainWindow *parent) : mainWindow(parent)
{    
    QVBoxLayout *mainLayout = new QVBoxLayout;

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
}

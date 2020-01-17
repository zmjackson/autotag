#-------------------------------------------------
#
# Project created by QtCreator 2019-12-15T16:49:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autotag
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += console

INCLUDEPATH += /home/zachary/Projects/glm

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dataframe3d.cpp \
    datapoint.cpp \
    utilities.cpp \
    dataviewwidget3d.cpp \
    tinyply.cpp \
    datatimeline.cpp \
    controlwidget.cpp \
    jsoncpp.cpp

HEADERS += \
        mainwindow.h \
    dataframe3d.h \
    tinyply.h \
    datapoint.h \
    dataviewwidget3d.h \
    datatimeline.h \
    controlwidget.h \
    json/json.h \
    trackedobject.h

FORMS += \
        mainwindow.ui \
    form.ui

DISTFILES += \
    vertex.vert \
    fragment.frag \
    example_cube-binary.ply \
    example_cube-ascii.ply \
    lidar_test.ply

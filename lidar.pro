#-------------------------------------------------
#
# Project created by QtCreator 2014-06-23T08:45:27
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH += "/usr/include/vtk-5.8/" \
               "/usr/include/pcl-1.7/" \
               "/usr/include/liblas/"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lidar
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Filters/GSDFilter.cpp \
    DataStructures/Grid.cpp \
    Dialogs/DialogGSDFilter.cpp

HEADERS  += MainWindow.h \
    Filters/GSDFilter.h \
    DataTypes.h \
    DataStructures/Grid.h \
    Dialogs/DialogGSDFilter.h

FORMS    += MainWindow.ui \
    Dialogs/DialogGSDFilter.ui

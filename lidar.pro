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
    Dialogs/DialogFilter.cpp \
    Filters/Filter.cpp \
    Filters/StandardDeviation.cpp \
    Filters/AverageDeviation.cpp \
    Forms/FormAverageDeviation.cpp \
    Forms/FormStandardDeviation.cpp

HEADERS  += MainWindow.h \
    DataTypes.h \
    Dialogs/DialogFilter.h \
    Filters/Filter.h \
    Filters/StandardDeviation.h \
    Filters/AverageDeviation.h \
    Forms/FormAverageDeviation.h \
    Forms/FormStandardDeviation.h

FORMS    += MainWindow.ui \
    Dialogs/DialogFilter.ui \
    Forms/FormAverageDeviation.ui \
    Forms/FormStandardDeviation.ui

#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T16:04:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTOpenCVWidget
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    CVImageWidget.cpp \
    FrameRenderWindow.cpp \
    RenderPane.cpp

HEADERS  += MainWindow.h \
    CVImageWidget.h \
    FrameRenderWindow.h \
    RenderPane.h


INCLUDEPATH += /usr/local/include/opencv2
LIBS += -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui

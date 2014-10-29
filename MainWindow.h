#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FrameRenderWindow.h"

class MainWindow : public QMainWindow, FrameRenderWindowCallback
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateRender();
    void allMouseEvent(FrameRenderWindow *win, QMouseEvent *e);
    void allKeyboardEvent(FrameRenderWindow *win, QKeyEvent *e, bool pressed);

    FrameRenderWindow *renderWindow;
    cv::Mat image;
    std::vector<cv::Point2f> listPoints;
};

#endif // MAINWINDOW_H

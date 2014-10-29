#ifndef FRAMERENDERWINDOW_H
#define FRAMERENDERWINDOW_H

#include "CVImageWidget.h"
#include "RenderPane.h"

class FrameRenderWindow;

class FrameRenderWindowCallback
{
public:
    virtual void updateRender() = 0;
    virtual void allMouseEvent(FrameRenderWindow *win, QMouseEvent *e) = 0;
    virtual void allKeyboardEvent(FrameRenderWindow *win, QKeyEvent *e, bool pressed) = 0;
};

class FrameRenderWindow : public CVImageWidget
{
    Q_OBJECT
public:
    FrameRenderWindow(QWidget *win, FrameRenderWindowCallback *callback);
    RenderPane *renderPane;
    cv::Point2f mouse;
    cv::Point2f oldMouse;

    FrameRenderWindowCallback *callback;
protected:
    void resizeEvent ( QResizeEvent * event );
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // FRAMERENDERWINDOW_H

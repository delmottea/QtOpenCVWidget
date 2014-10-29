#include "FrameRenderWindow.h"
#include <QMouseEvent>

FrameRenderWindow::FrameRenderWindow(QWidget *win, FrameRenderWindowCallback *callback)
    :CVImageWidget(win), callback(callback)
{
    renderPane = new RenderPane();
}

void FrameRenderWindow::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    qreal sc = pow(1.25, numSteps);
    QPoint pos1 = event->pos();
    QPointF center(pos1.x()/renderPane->zoom+renderPane->offsetX, pos1.y()/renderPane->zoom+renderPane->offsetY);
    if(renderPane->zoom * sc > 100)
        renderPane->zoom = 100;
    else if(renderPane->zoom * sc < 0.01)
        renderPane->zoom = 0.01;
    else renderPane->zoom *= sc;
    QPoint pos2((center.x()-renderPane->offsetX)*renderPane->zoom, (center.y()-renderPane->offsetY)*renderPane->zoom);
    QPoint dp = pos2 - pos1;
    renderPane->offsetX += dp.x()/renderPane->zoom;
    renderPane->offsetY += dp.y()/renderPane->zoom;
    event->accept();
    callback->updateRender();
}

void FrameRenderWindow::resizeEvent ( QResizeEvent * event )
{
    CVImageWidget::resizeEvent(event);
    if(size().width() != 0 && size().height() != 0)
    {
        renderPane->resize(size().width(), size().height());
        callback->updateRender();
    }
}

void FrameRenderWindow::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    callback->allMouseEvent(this, event);
}

void FrameRenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    callback->allMouseEvent(this, event);
}

void FrameRenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    callback->allMouseEvent(this, event);
}

void FrameRenderWindow::keyPressEvent(QKeyEvent *event)
{
    callback->allKeyboardEvent(this, event, true);
}

void FrameRenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    callback->allKeyboardEvent(this, event, false);
}


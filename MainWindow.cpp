#include "MainWindow.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    image = cv::imread("data/lena.jpg");
    renderWindow = new FrameRenderWindow(this, this);
    this->setCentralWidget(renderWindow);

    resize(QSize(500,500));
}

MainWindow::~MainWindow()
{

}


void MainWindow::updateRender()
{
    RenderPane *renderPane = renderWindow->renderPane;
    renderPane->clear();
    renderPane->drawImg(image);
    for(int i = 0; i < listPoints.size(); i++)
        renderPane->drawCircle(listPoints[i].x, listPoints[i].y, 3, 255, 255, 255, false);

    renderWindow->showImage(renderPane->image);
}

void MainWindow::allMouseEvent(FrameRenderWindow *win, QMouseEvent *e)
{
    cv::Point2f mouse = cv::Point2f(win->renderPane->realToLocalX(e->x()), win->renderPane->realToLocalY(e->y()));
    bool leftDown = (e->buttons() & Qt::LeftButton);
    bool leftClick = (e->button() == Qt::LeftButton && leftDown);
    bool rightDown = (e->buttons() & Qt::RightButton);
    bool rightClick = (e->button() == Qt::RightButton && rightDown);
    if(leftClick)
    {
        listPoints.push_back(mouse);
        updateRender();
    }
}

void MainWindow::allKeyboardEvent(FrameRenderWindow *win, QKeyEvent *e, bool pressed)
{
    if(pressed && !e->isAutoRepeat())
    {
        if(e->key() == Qt::Key_Delete)
        {
            if(listPoints.size() > 0)
                listPoints.erase(listPoints.begin()+(listPoints.size()-1));
            updateRender();
        }
    }
    else e->ignore();
}

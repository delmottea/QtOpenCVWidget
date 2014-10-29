#include "CVImageWidget.h"
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>

CVImageWidget::CVImageWidget(QWidget *parent)
    : QWidget(parent)
{
    _tmp = cv::Mat(1,1, CV_8UC3);
    refreshImage();
}

void CVImageWidget::showImage(const cv::Mat& image)
{
    cv::Mat src;
    // Convert the image to the RGB888 format
    switch (image.type()) {
    case CV_8UC1:
        cvtColor(image, src, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(image, src, CV_BGR2RGB);
        break;
    }

    if(size().width() != 0 && size().height() != 0)
    {
        cv::resize(src, _tmp, cv::Size(size().width(), size().height()), 0, 0, cv::INTER_CUBIC);
        refreshImage();
    }
}

void CVImageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
    painter.end();
}

void CVImageWidget::resizeEvent ( QResizeEvent * event )
{
    cv::Mat dst;
    if(event->size().width() != 0 && event->size().height() != 0)
    {
        cv::resize(_tmp, dst, cv::Size(event->size().width(), event->size().height()), 0, 0, cv::INTER_CUBIC);
        _tmp = dst;
        refreshImage();
    }
}

void CVImageWidget::refreshImage()
{
    // QImage needs the data to be stored continuously in memory
    assert(_tmp.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    repaint();
}

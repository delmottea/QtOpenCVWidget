#ifndef CVIMAGEWIDGET_H
#define CVIMAGEWIDGET_H

#include <QWidget>
#include <opencv.hpp>

/**
 * @brief The CVImageWidget class
 *  Object that allows to draw an image with the openCV Mat class
 */
class CVImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVImageWidget(QWidget *parent = 0);
    void showImage(const cv::Mat& image);

    QImage _qimage;
    cv::Mat _tmp;

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent ( QResizeEvent * event );
    void refreshImage();
};

#endif // CVIMAGEWIDGET_H

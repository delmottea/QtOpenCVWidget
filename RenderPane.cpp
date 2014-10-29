#include "RenderPane.h"
#include <QDebug>

#define LocalToReal(x,y,w,h) (x-offsetX)*zoom, (y-offsetY)*zoom, w*zoom, h*zoom
#define LocalToRealPos(x,y) (x-offsetX)*zoom, (y-offsetY)*zoom

RenderPane::RenderPane(int w, int h)
{
    image = cv::Mat(w, h, CV_8UC3);
    zoom = 1.0;
    offsetX = 0;
    offsetY = 0;
}


void RenderPane::resize(int w, int h)
{
    if(w != 0 && h != 0)
    {
        cv::Mat dst;
        cv::resize(image, dst, cv::Size(w,h), 0, 0, cv::INTER_CUBIC);
        image = dst;
    }
}


void RenderPane::clear(unsigned char r, unsigned char g, unsigned char b)
{
    fillRectReal(0,0,image.cols, image.rows, r, g, b);
}

void RenderPane::fillRectReal(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
    if(x < 0)
    {
        w += x;
        x = 0;
    }
    if(y < 0)
    {
        h += y;
        y = 0;
    }
    if(x + w > image.cols)
        w = image.cols-x;
    if(y + h > image.rows)
        h = image.rows-y;

    for(int j = y; j < y+h; j++)
        for(int i = x; i < x+w; i++)
            setPixelRealUnsafe(i,j, r, g, b);
}

void RenderPane::fillRect(float x, float y, float w, float h, unsigned char r, unsigned char g, unsigned char b)
{
    fillRectReal(LocalToReal(x,y,w,h), r, g, b);
}

void RenderPane::drawImgReal(cv::Mat img, int x, int y, int w, int h, float alpha)
{
    qDebug() << "drawImgReal(img, " << x << "," << y << "," << w << "," << h << ")";
    if(img.cols == 0 || img.rows == 0)
        return;
    if(w == 0)
        w = img.cols;
    if(h == 0)
        h = img.rows;
    int x1 = (x >= 0 ? x : 0);
    int y1 = (y >= 0 ? y : 0);
    int x2 = x+w;
    if(x2 > image.cols)
        x2 = image.cols;
    int y2 = y+h;
    if(y2 > image.rows)
        y2 = image.rows;
    if(x2 <= x1 || y2 <= y1)
        return;
    int x1b = std::max((int)floor((x1-x)*img.cols/w),0);
    int y1b = std::max((int)floor((y1-y)*img.rows/h),0);
    int x2b = std::min((int)ceil(float(x2-x)*img.cols/w),img.cols);
    int y2b = std::min((int)ceil(float(y2-y)*img.rows/h),img.rows);
    if(x1b >= x2b)
        x2b = x1b+1;
    if(y1b >= y2b)
        y2b = y1b+1;
    qDebug() << "size : " << x1b << y1b << x2b << y2b;
    qDebug() << "resize : " << x1 << y1 << x2 << y2;

    x = x1;
    y = y1;
    cv::Mat dst;
    //qDebug() << type2str(img.type()).c_str();
    qDebug() << x1b << y1b << x2b-x1b << y2b-y1b;
    cv::resize(img(cv::Rect(x1b, y1b, x2b-x1b, y2b-y1b)), dst, cv::Size(x2-x1,y2-y1), 0, 0, cv::INTER_CUBIC);
    img = dst;
    float beta = 1.0-alpha;
    qDebug() <<"draw";
    if(img.type() == CV_8UC3)
    {
        for(int j = y1; j < y2; j++)
        {
            int offset1 = (x1+j*image.cols)*3;
            int offset1b = (x2+j*image.cols)*3;
            int offset2 = ((x1-x)+(j-y)*img.cols)*3;
            while(offset1!=offset1b)
            {
                image.data[offset1] = (int)image.data[offset1]*beta+img.data[offset2++]*alpha;
                offset1++;
            }
        }
    }
    else if(img.type() == CV_8UC1)
    {
        for(int j = y1; j < y2; j++)
        {
            int offset1 = (x1+j*image.cols)*3;
            int offset1b = (x2+j*image.cols)*3;
            int offset2 = ((x1-x)+(j-y)*img.cols);
            while(offset1!=offset1b)
            {
                unsigned char v = img.data[offset2++];
                image.data[offset1] = (int)image.data[offset1]*beta+v*alpha;
                offset1++;
                image.data[offset1] = (int)image.data[offset1]*beta+v*alpha;
                offset1++;
                image.data[offset1] = (int)image.data[offset1]*beta+v*alpha;
                offset1++;
            }
        }
    }
    else if(img.type() == CV_32FC1)
    {
        for(int j = y1; j < y2; j++)
        {
            int offset1 = (x1+j*image.cols)*3;
            int offset1b = (x2+j*image.cols)*3;
            int offset2 = ((x1-x)+(j-y)*img.cols);
            while(offset1!=offset1b)
            {
                int v = 255*((float*)img.data)[offset2++];
                image.data[offset1] = std::max(0,std::min(255,(int)(image.data[offset1]*beta+v*alpha)));
                offset1++;
                image.data[offset1] = std::max(0,std::min(255,(int)(image.data[offset1]*beta+v*alpha)));
                offset1++;
                image.data[offset1] = std::max(0,std::min(255,(int)(image.data[offset1]*beta+v*alpha)));
                offset1++;
            }
        }
    }
    qDebug() << "end drawImgReal(img, " << x << "," << y << "," << w << "," << h << ")";
}

void RenderPane::drawImg(cv::Mat img, float x, float y, float w, float h, float alpha)
{
    if(w == 0)
        w = img.cols;
    if(h == 0)
        h = img.rows;
    drawImgReal(img, LocalToReal(x,y,w,h), alpha);
}


void RenderPane::drawCircleReal(int cx, int cy, int radius, unsigned char r, unsigned char g, unsigned char b, bool fill)
{
    if(fill)
    {
        int x1 = (cx-radius > 0 ? cx-radius : 0);
        int y1 = (cy-radius > 0 ? cy-radius : 0);
        int x2 = cx+radius+1;
        if(x2 > image.cols)
            x2 = image.cols;
        int y2 = cy+radius+1;
        if(y2 > image.rows)
            y2 = image.rows;
        int squareRadius = radius*radius;
        for(int i = x1; i < x2; i++)
        {
            int squareDx = (i-cx)*(i-cx);
            for(int j = y1; j < y2; j++)
                if(squareDx+(j-cy)*(j-cy) <= squareRadius)
                    setPixelRealUnsafe(i,j,r,g,b);
        }
    }
    else cv::circle(image, cv::Point(cx, cy), radius, cv::Scalar(r,g,b));
}

void RenderPane::drawCircle(float cx, float cy, float radius, unsigned char r, unsigned char g, unsigned char b, bool fill)
{
    drawCircleReal(LocalToRealPos(cx,cy), zoom*radius, r, g, b, fill);
}

void RenderPane::drawQuadReal(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
    drawLineReal(x1,y1,x2,y1,r,g,b);
    drawLineReal(x2,y1,x2,y2,r,g,b);
    drawLineReal(x1,y2,x2,y2,r,g,b);
    drawLineReal(x1,y1,x1,y2,r,g,b);
}

void RenderPane::drawQuad(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b)
{
    drawQuadReal(LocalToRealPos(x1,y1), LocalToRealPos(x2,y2), r, g, b);
}

void RenderPane::drawLineReal(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
    float dx = x2-x1;
    float dy = y2-y1;
    float dist = sqrt(dx*dx+dy*dy);
    dx /= dist;
    dy /= dist;
    for(float i = 0; i <= dist; i += 0.5)
        setPixelReal(x1+i*dx,y1+i*dy, r, g, b);
}

void RenderPane::drawLine(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b)
{
    drawLineReal(LocalToRealPos(x1,y1), LocalToRealPos(x2,y2), r, g, b);
}

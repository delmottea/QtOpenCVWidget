#ifndef RENDERPANE_H
#define RENDERPANE_H

#include <opencv.hpp>

//operations suffixed with "Real" are in normal coordinate space, others are in modified space (zoom and offset)
//operations suffixed with "Unsafe" doesn't have bound verification (but are faster)
/**
 * @brief The RenderPane class
 *  list of functions of drawing
 */
class RenderPane
{
public:
    RenderPane(int w = 10, int h = 10);
    void resize(int w, int h);
    void clear(unsigned char r=0, unsigned char g=0, unsigned char b=0);
    void fillRectReal(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
    void fillRect(float x, float y, float w, float h, unsigned char r, unsigned char g, unsigned char b);
    //if w = 0 or h = 0, the width and height are img.cols and img.rows
    void drawImgReal(cv::Mat img, int x = 0, int y = 0, int w = 0, int h = 0, float alpha = 1.0);
    //if w = 0 or h = 0, the width and height are img.cols and img.rows
    void drawImg(cv::Mat img, float x = 0, float y = 0, float w = 0, float h = 0, float alpha = 1.0);

    void drawQuadReal(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b);
    void drawQuad(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b);

    void drawCircleReal(int cx, int cy, int radius, unsigned char r, unsigned char g, unsigned char b, bool fill = true);
    void drawCircle(float cx, float cy, float radius, unsigned char r, unsigned char g, unsigned char b, bool fill = true);

    void drawLineReal(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b);
    void drawLine(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b);

    inline void setPixelRealUnsafe(int x, int y, unsigned char r, unsigned char g, unsigned char b)
    {
        int offset = (x+y*image.cols)*3;
        image.data[offset] = b;
        image.data[offset+1] = g;
        image.data[offset+2] = r;
    }

    inline void setPixelReal(int x, int y, unsigned char r, unsigned char g, unsigned char b)
    {
        if(x >= 0 && x < image.cols && y >= 0 && y < image.rows)
            setPixelRealUnsafe(x,y,r,g,b);
    }

    inline void getPixelRealUnsafe(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b) const
    {
        int offset = (x+y*image.cols)*3;
        b = image.data[offset];
        g = image.data[offset+1];
        r = image.data[offset+2];
    }

    inline void getPixelReal(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b) const
    {
        if(x >= 0 && x < image.cols && y >= 0 && y < image.rows)
            getPixelRealUnsafe(x,y,r,g,b);
        else r = g = b = 0;
    }

    inline float realToLocalX(int x) const
    {
         return x/zoom+offsetX;
    }

    inline float realToLocalY(int y) const
    {
         return y/zoom+offsetY;
    }


    cv::Mat image;
    float zoom;
    float offsetX, offsetY;
};

#endif // RENDERPANE_H

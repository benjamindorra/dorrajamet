#include "Frame.h"

namespace render {
    Frame::Frame ()
    {

    }
    Frame::Frame (Canvas * canvas)
    {
        this->canvas=canvas;
        this->x=0;
        this->y=0;
        this->width=canvas->getWidth();
        this->height=canvas->getHeight();
    }
    Frame::Frame (Canvas * canvas, int width, int height)
    {
        this->canvas=canvas;
        this->width=width;
        this->height=height;
        this->x=(canvas->getWidth()-width)/2;
        this->y=(canvas->getHeight()-height)/2;
    }
    Frame::Frame (Canvas * canvas, int width, int height, int x, int y)
    {
        this->canvas=canvas;
        this->width=width;
        this->height=height;
        this->x=x;
        this->y=y;
    }
    Frame::~Frame ()
    {

    }

    // Setters and Getters
    int Frame::getWidth ()
    {
        return this->width;
    }
    int Frame::getHeight ()
    {
        return this->height;
    }
    int Frame::getX ()
    {
        return this->x;
    }
    int Frame::getY ()
    {
        return this->y;
    }

}


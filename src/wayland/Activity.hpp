#ifndef SRC_WAYLAND_ACTIVITY_HPP_
#define SRC_WAYLAND_ACTIVITY_HPP_

#include <iostream>

#include "Display.hpp"
#include "Surface.hpp"
#include "SharedFile.hpp"

using Wayland::Display;
using Wayland::FrameCallbackListener;
using Wayland::SharedBuffer;
using Wayland::SharedFile;
using Wayland::Surface;

class Rect
{
public:
    int left;
    int top;
    int right;
    int bottom;

    int width;
    int height;

    Rect(int w, int h)
    {
        width = w;
        height = h;
    }

    Rect(int l, int t, int r, int b)
    {
        left = l;
        top = t;
        right = r;
        bottom = b;

        width = right - left;
        height = bottom - top;
    }
};

class Rgb
{
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    uint32_t bpp;

    uint32_t pixelFormat;
    Rgb()
    {
        bpp = 32;
        pixelFormat = WL_SHM_FORMAT_XRGB8888;
    }

    Rgb(uint8_t ir, uint8_t ig, uint8_t ib, uint8_t ia)
    {
        r = ir;
        g = ig;
        b = ib;
        a = ia;

        bpp = 32;
        pixelFormat = WL_SHM_FORMAT_XRGB8888;
    }
};

class Activity : public FrameCallbackListener
{
private:
    std::shared_ptr<Display> mDisplay;
    std::shared_ptr<Surface> mSurface;
    std::shared_ptr<SharedFile> mSharedFile;
    std::shared_ptr<SharedBuffer> mSharedBuffer;

public:
    Rect mRect;
    Rgb mRgb;

    Activity(Rect r);
    ~Activity();

    void init();
    void release();

    void draw();
    void onFrameDisplayed();

    void updateData(Rgb rgb);
};

#endif
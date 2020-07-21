#ifndef SRC_WAYLAND_ACTIVITY_HPP_
#define SRC_WAYLAND_ACTIVITY_HPP_

#include <iostream>

#include "Display.hpp"
#include "Surface.hpp"
#include "SharedFile.hpp"
#include "Pointer.hpp"

using Wayland::Display;
using Wayland::FrameCallbackListener;
using Wayland::SharedBuffer;
using Wayland::SharedFile;
using Wayland::Surface;
using Wayland::Pointer;
using Wayland::PointerListener;

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

struct Rgba
{
    uint8_t x;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class FormatInfo
{
public:
    Rgba rgb;

    uint32_t bpp;

    uint32_t pixelFormat;
    FormatInfo()
    {
        bpp = 32;
        pixelFormat = WL_SHM_FORMAT_XRGB8888;
    }

    FormatInfo(uint8_t ix, uint8_t ir, uint8_t ig, uint8_t ib)
    {
        rgb.x = ix;
        rgb.r = ir;
        rgb.g = ig;
        rgb.b = ib;

        bpp = 32;
        pixelFormat = WL_SHM_FORMAT_XRGB8888;
    }
};

class Activity : public FrameCallbackListener, public PointerListener
{
private:
    std::string mName;

    std::shared_ptr<Display> mDisplay;
    std::shared_ptr<Surface> mSurface;
    std::shared_ptr<SharedFile> mSharedFile;
    std::shared_ptr<SharedBuffer> mSharedBuffer;
    std::shared_ptr<Pointer> mPointer;

public:
    Rect mRect;
    FormatInfo mFormatInfo;

    XenBackend::Log mLog;

    Activity(Rect r);
    ~Activity();

    void init();
    void release();

    void draw();
    void onFrameDisplayed();

    void updateData(FormatInfo finfo);

    int mMask = 0xFF;
    void buttonStateChanged(uint32_t serial, uint32_t time, uint32_t button, bool pressed);
};

#endif
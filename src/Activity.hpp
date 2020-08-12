#ifndef SRC_WAYLAND_ACTIVITY_HPP_
#define SRC_WAYLAND_ACTIVITY_HPP_

#include <iostream>

#include "wayland/Display.hpp"
#include "wayland/Surface.hpp"
#include "wayland/SharedFile.hpp"
#include "wayland/Pointer.hpp"
#include "egl/EglWrapper.hpp"
#include "opengl/OpenGLRender.hpp"

using Wayland::Display;
using Wayland::FrameCallbackListener;
using Wayland::Pointer;
using Wayland::PointerListener;
using Wayland::SharedBuffer;
using Wayland::SharedFile;
using Wayland::ShellSurface;
using Wayland::Surface;
using egl::EglWrapper;
using gl::OpenGLRender;

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
    std::shared_ptr<ShellSurface> mShellSurface;
    std::shared_ptr<SharedFile> mSharedFile;
    std::shared_ptr<SharedBuffer> mSharedBuffer;
    std::shared_ptr<Pointer> mPointer;

    std::shared_ptr<EglWrapper> mEglWrapper;

    std::shared_ptr<OpenGLRender> mGLRender;

public:
    Rect mRect;
    FormatInfo mFormatInfo;

    XenBackend::Log mLog;

    Activity(Rect r);
    ~Activity();

    void init();
    void release();

    std::shared_ptr<Surface> getSurface()
    {
        return mSurface;
    }

    std::shared_ptr<EglWrapper> getEglWrapper() {
        return mEglWrapper;
    }

    void draw();
    void onFrameDisplayed();

    void updateData(FormatInfo finfo);

    int mMask = 0xFF;
    uint32_t mSerial;
    bool mPressed;

    void pointerEnter(uint32_t serial, wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy) { mSerial = serial; };
    void pointerLeave(uint32_t serial, wl_surface *surface);
    void pointerMotion(uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
    void pointerButton(uint32_t serial, uint32_t time, uint32_t button, bool pressed);
    void pointerAxis(uint32_t time, uint32_t axis, wl_fixed_t value){};
    void pointerFrame(){};
    void pointerAxisSource(uint32_t axis_source){};
    void pointerAxisStop(uint32_t time, uint32_t axis){};
    void pointerAxisDiscrete(uint32_t axis, int32_t discrete){};
};

#endif
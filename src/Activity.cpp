#include <iostream>

#include "Activity.hpp"

using std::exception;

Activity::Activity(Rect r) : mRect(r), mLog("Activity"), mName("unamed-Activity")
{
    init();
}

Activity::~Activity()
{
    release();
}

void Activity::init()
{
    mDisplay.reset(new Display());

    mDisplay->start();

    mSurface = mDisplay->getCompositor()->createSurface();
    mSurface->setListener(this);

    mShellSurface = mDisplay->getShell()->getShellSurface(mSurface);

    mShellSurface->setTopLevel();
    mShellSurface->setTitle(mName);

    mSharedFile = mDisplay->getSharedMemory()->createSharedFile(mRect.width, mRect.height, mFormatInfo.bpp);

    mSharedBuffer = mDisplay->getSharedMemory()->createSharedBuffer(mSharedFile, mRect.width, mRect.height,
                                                                    mSharedFile->getStride(),
                                                                    mFormatInfo.pixelFormat);

    mPointer = mDisplay->pointer();
    if (mPointer)
    {
        mPointer->addPointerListener(mName, this);
    }

    mEglWrapper.reset(new EglWrapper(mDisplay));
    mEglWrapper->init(this);

    mGLRender.reset(new OpenGLRender(mDisplay));
    mGLRender->init(this);

    LOG(mLog, DEBUG) << "Buffer size: " << mSharedFile->getSize();
}

void Activity::release()
{
    mDisplay.reset();
}

void Activity::draw()
{
    try
    {
        LOG(mLog, DEBUG) << "drawFrame Buffer size: " << mSharedFile->getSize();
        while (true)
        {
            mGLRender->render(this, 0);
            wl_display_dispatch_pending(mDisplay->display());
        }
    }
    catch (const exception &e)
    {
        LOG(mLog, ERROR) << e.what();
    }
}

void Activity::onFrameDisplayed()
{
    wl_display_dispatch_pending(mDisplay->display());
    LOG(mLog, DEBUG) << "onFrameDisplayed Buffer size: " << mSharedFile->getSize();
    draw();
}

void Activity::updateData(FormatInfo finfo)
{
    Rgba *data = static_cast<Rgba *>(mSharedFile->getBuffer());

    for (size_t i = 0; i < mSharedFile->getSize() / sizeof(Rgba); i++)
    {
        data[i].x = finfo.rgb.x;
        data[i].r = finfo.rgb.r;
        data[i].g = finfo.rgb.g;
        data[i].b = finfo.rgb.b;
    }
}

void Activity::pointerLeave(uint32_t serial, wl_surface *surface)
{
    mPressed = false;
};

void Activity::pointerButton(uint32_t serial, uint32_t time, uint32_t button, bool pressed)
{
    mPressed = pressed;

    LOG(mLog, DEBUG) << __func__ << " " << mPressed;

    if (pressed)
    {
        mMask = mMask << 8;
    }
    if ((mMask & 0xFFFFFFFF) == 0x0)
    {
        mMask = 0xFF;
    }
}

void Activity::pointerMotion(uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
    LOG(mLog, DEBUG) << __func__ << " " << mPressed;
    if (mShellSurface)
    {
        if (mPressed)
        {
            mShellSurface->move(mDisplay->getSeat().get(), mSerial);
        }
    }
}
#include <iostream>

#include "Activity.hpp"

using std::exception;

Activity::Activity(Rect r) : mRect(r), mLog("Activity")
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

    auto shellSurface = mDisplay->getShell()->getShellSurface(mSurface);

    shellSurface->setTopLevel();

    mSharedFile = mDisplay->getSharedMemory()->createSharedFile(mRect.width, mRect.height, mFormatInfo.bpp);

    mSharedBuffer = mDisplay->getSharedMemory()->createSharedBuffer(mSharedFile, mRect.width, mRect.height,
                                                                    mSharedFile->getStride(),
                                                                    mFormatInfo.pixelFormat);

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
        mSurface->draw(mSharedBuffer);
    }
    catch (const exception &e)
    {
        LOG(mLog, ERROR) << e.what();
    }
}

void Activity::onFrameDisplayed()
{
    updateData(FormatInfo(0x00, 0xff, 0x00, 0x00)); //xrgb
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
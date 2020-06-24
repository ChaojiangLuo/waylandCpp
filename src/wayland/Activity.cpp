#include <iostream>

#include "Activity.hpp"

using std::exception;

Activity::Activity(Rect r) : mRect(r)
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

    mSharedFile = mDisplay->getSharedMemory()->createSharedFile(mRect.width, mRect.height, mRgb.bpp);

    mSharedBuffer = mDisplay->getSharedMemory()->createSharedBuffer(mSharedFile, mRect.width, mRect.height,
                                                                    mSharedFile->getStride(),
                                                                    mRgb.pixelFormat);
}

void Activity::release()
{
    mDisplay.reset();
}

void Activity::draw()
{
    try
    {
        LOG("Main", DEBUG) << "drawFrame Buffer size: " << mSharedFile->getSize();
        mSurface->draw(mSharedBuffer);
    }
    catch (const exception &e)
    {
        LOG("Main", ERROR) << e.what();
    }
}

void Activity::onFrameDisplayed()
{
    //updateData(Rgb(0xFF, 0x00, 0x00, 0x00));
    draw();
}

void Activity::updateData(Rgb rgb)
{
    Rgb *data = static_cast<Rgb *>(mSharedFile->getBuffer());

    for (size_t i = 0; i < mSharedFile->getSize() / sizeof(Rgb); i++)
    {
        data[i].a = rgb.a;
        data[i].r = rgb.r;
        data[i].g = rgb.g;
        data[i].b = rgb.b;
    }
}
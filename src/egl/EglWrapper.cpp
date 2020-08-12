#include <assert.h>

#include <wayland-egl.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "wayland/Display.hpp"
#include "Activity.hpp"

#include "EglWrapper.hpp"

EglWrapper::EglWrapper(std::shared_ptr<Display> display) : mDisplay(display)
{
}

EglWrapper::~EglWrapper()
{
}

/*******************************************************************************
 * Private
 ******************************************************************************/
void EglWrapper::init(Activity *activity)
{
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE};

    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE};

    EGLint major, minor, n, count, i, size;
    EGLConfig *configs;
    EGLBoolean ret;

    eglDisplay = eglGetDisplay(getDisplay()->display());
    assert(eglDisplay);

    ret = eglInitialize(eglDisplay, &major, &minor);
    assert(ret == EGL_TRUE);
    ret = eglBindAPI(EGL_OPENGL_ES_API);
    assert(ret == EGL_TRUE);

    if (!eglGetConfigs(eglDisplay, NULL, 0, &count) || count < 1)
        assert(0);

    configs = new EGLConfig[count];
    assert(configs);

    ret = eglChooseConfig(eglDisplay, config_attribs,
                          configs, count, &n);
    assert(ret && n >= 1);

    for (i = 0; i < n; i++)
    {
        EGLint nativeVisualId = 0;
        eglGetConfigAttrib(eglDisplay, configs[i], EGL_NATIVE_VISUAL_ID, &nativeVisualId);
        if (nativeVisualId > 0 && activity->mFormatInfo.pixelFormat == nativeVisualId)
        {
            eglConfig = configs[i];
            break;
        }
    }

    if (i == n)
    {
        eglConfig = configs[0];
    }

    eglContext = eglCreateContext(eglDisplay,
                                  eglConfig,
                                  EGL_NO_CONTEXT, context_attribs);
    assert(eglContext);

    wlEglWindow = wl_egl_window_create(activity->getSurface()->surface(), activity->mRect.width, activity->mRect.height);

    eglSurface = eglCreateWindowSurface(eglDisplay,
                                        eglConfig,
                                        wlEglWindow, NULL);

    ret = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    assert(ret == EGL_TRUE);
}

void EglWrapper::release()
{
}
#ifndef SRC_WAYLAND_EGL_WRAPPER_HPP_
#define SRC_WAYLAND_EGL_WRAPPER_HPP_

#include <wayland-client.h>
#include <wayland-egl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

using Wayland::Display;

class Activity;
namespace egl
{
    class EglWrapper
    {
    private:
        std::shared_ptr<Display> mDisplay;

    public:
        void init(Activity *activity);
        void release();

        EGLDisplay eglDisplay;
        EGLContext eglContext;
        EGLConfig eglConfig;
        EGLSurface eglSurface;

        wl_egl_window *wlEglWindow;

        EglWrapper(std::shared_ptr<Display> display);
        ~EglWrapper();

        std::shared_ptr<Display> getDisplay()
        {
            return mDisplay;
        }
    };
} // namespace egl

#endif
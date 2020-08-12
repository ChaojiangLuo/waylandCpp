#ifndef SRC_WAYLAND_OPENGLRENDER_HPP_
#define SRC_WAYLAND_OPENGLRENDER_HPP_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

using Wayland::Display;

class Activity;
namespace gl
{
    class OpenGLRender
    {
    private:
        std::shared_ptr<Display> mDisplay;

        XenBackend::Log mLog;

        char *vert_shader_text =
            "uniform mat4 rotation;\n"
            "attribute vec4 pos;\n"
            "attribute vec4 color;\n"
            "varying vec4 v_color;\n"
            "void main() {\n"
            "  gl_Position = rotation * pos;\n"
            "  v_color = color;\n"
            "}\n";

        char *frag_shader_text =
            "precision mediump float;\n"
            "varying vec4 v_color;\n"
            "void main() {\n"
            "  gl_FragColor = v_color;\n"
            "}\n";

    public:
        void init(Activity *activity);
        void release();

        GLuint rotation_uniform;
        GLuint pos;
        GLuint col;

        uint32_t benchmark_time, frames;

        OpenGLRender(std::shared_ptr<Display> display);
        ~OpenGLRender();

        GLuint createShader(const char *source, GLenum shader_type);

        void render(Activity *activity, uint32_t time);
    };

} // namespace gl

#endif
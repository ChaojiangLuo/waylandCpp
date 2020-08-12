#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <signal.h>

#include <sys/time.h>

#include "wayland/Display.hpp"
#include "Activity.hpp"

#include "OpenGLRender.hpp"

OpenGLRender::OpenGLRender(std::shared_ptr<Display> display) : mDisplay(display), mLog("GLRender")
{
}

OpenGLRender::~OpenGLRender()
{
}

GLuint OpenGLRender::createShader(const char *source, GLenum shader_type)
{
    GLuint shader;
    GLint status;

    shader = glCreateShader(shader_type);
    assert(shader != 0);

    glShaderSource(shader, 1, (const char **)&source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[1000];
        GLsizei len;
        glGetShaderInfoLog(shader, 1000, &len, log);
        fprintf(stderr, "Error: compiling %s: %*s\n",
                shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                len, log);
        exit(1);
    }

    return shader;
}

/*******************************************************************************
 * Private
 ******************************************************************************/
void OpenGLRender::init(Activity *activity)
{
    GLuint frag, vert;
    GLuint program;
    GLint status;

    frag = createShader(frag_shader_text, GL_FRAGMENT_SHADER);
    vert = createShader(vert_shader_text, GL_VERTEX_SHADER);

    program = glCreateProgram();

    glAttachShader(program, frag);
    glAttachShader(program, vert);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char log[1000];
        GLsizei len;
        glGetProgramInfoLog(program, 1000, &len, log);
        fprintf(stderr, "Error: linking:\n%*s\n", len, log);
        exit(1);
    }

    glUseProgram(program);

    pos = 0;
    col = 1;

    glBindAttribLocation(program, pos, "pos");
    glBindAttribLocation(program, col, "color");
    glLinkProgram(program);

    rotation_uniform = glGetUniformLocation(program, "rotation");

    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRender::render(Activity *activity, uint32_t time)
{
    LOG(mLog, DEBUG) << "render size: "
                     << " will time";
    static const GLfloat verts[3][2] = {
        {-0.5, -0.5},
        {0.5, -0.5},
        {0, 0.5}};
    static const GLfloat colors[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}};
    GLfloat angle;
    GLfloat rotation[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};
    static const int32_t speed_div = 5, benchmark_interval = 5;
    struct wl_region *region;
    EGLint rect[4];
    EGLint buffer_age = 0;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    if (frames == 0)
        benchmark_time = time;
    if (time - benchmark_time > (benchmark_interval * 1000))
    {
        printf("%d frames in %d seconds: %f fps\n",
               frames,
               benchmark_interval,
               (float)frames / benchmark_interval);
        benchmark_time = time;
        frames = 0;
    }

    LOG(mLog, DEBUG) << "render size: "
                     << " will " << frames;

    angle = (time / speed_div) % 360 * M_PI / 180.0;
    rotation[0][0] = cos(angle);
    rotation[0][2] = sin(angle);
    rotation[2][0] = -sin(angle);
    rotation[2][2] = cos(angle);

    LOG(mLog, DEBUG) << "render size: "
                     << " will " << frames;

    glViewport(0, 0, activity->mRect.width, activity->mRect.height);

    LOG(mLog, DEBUG) << "render size: "
                     << " will " << activity->mRect.width << " " << activity->mRect.height
                     << " rotation_uniform " << rotation_uniform << " rotation " << rotation;

    glUniformMatrix4fv(rotation_uniform, 1, GL_FALSE,
                       (GLfloat *)rotation);  

    LOG(mLog, DEBUG) << "render size: "
                     << " no glClear " << frames;

    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
    glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(col);

    LOG(mLog, DEBUG) << "render size: "
                     << " will glDrawArrays " << frames;

    glDrawArrays(GL_TRIANGLES, 0, 3);

    LOG(mLog, DEBUG) << "render size: "
                     << " will glDrawArrays done " << frames;

    glDisableVertexAttribArray(pos);
    glDisableVertexAttribArray(col);

    LOG(mLog, DEBUG) << "render size: "
                     << " will wl_surface_set_opaque_region";

    wl_surface_set_opaque_region(activity->getSurface()->surface(), NULL);

    eglSwapBuffers(activity->getEglWrapper()->eglDisplay, activity->getEglWrapper()->eglSurface);

    LOG(mLog, DEBUG) << "render size: "
                     << " eglSwapBuffers done";

    frames++;
}
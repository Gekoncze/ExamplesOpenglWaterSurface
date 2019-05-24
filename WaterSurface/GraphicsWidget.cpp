#include "GraphicsWidget.h"
#include "Logger.h"
#include <qevent.h>
#include <chrono>
#include "graphics.h"

const float CAMERA_ROTATION_SPEED = 0.01f;

using namespace cz_mg_logger;

GraphicsWidget::GraphicsWidget(QWidget* parent) : QOpenGLWidget(parent), timer(this)
{
    Logger::log("Graphics widget constructor!");

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    setFormat(format);

    timer.setInterval(25);
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(stepEvent()));

    setMouseTracking(true);
    setFocus();
}

GraphicsWidget::~GraphicsWidget()
{
}

static void myDebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    Logger::log(std::string(message));
}

void GraphicsWidget::initializeGL()
{
    if (glewInit() != GLEW_OK) throw std::string("Could not init glew!");

    const GLubyte* version = glGetString(GL_VERSION);
    checkForErrors("Could not get OpenGL version.");
    Logger::log(std::string("Using version ") + ((char*)version));

    glDebugMessageCallback(myDebugCallback, nullptr);
    checkForErrors("Could not set debug callback.");

    initGraphics();
    timer.start();
    Logger::log("Timer started!");

    updateMatrix(width(), height());
}

void GraphicsWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    updateMatrix(width(), height());
}

void GraphicsWidget::paintGL()
{
    drawGraphics();
}

void GraphicsWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MouseButton::LeftButton)
    {
        drag = true;
    }
}

void GraphicsWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MouseButton::LeftButton) drag = false;
}

void GraphicsWidget::mouseMoveEvent(QMouseEvent* e)
{
    static int lx = 0;
    static int ly = 0;
    if (drag)
    {
        cameraYaw += (float)((e->x() - lx) * CAMERA_ROTATION_SPEED);
        cameraPitch += (float)((e->y() - ly) * CAMERA_ROTATION_SPEED);
        updateMatrix(width(), height());
    }
    lx = (int)e->x();
    ly = (int)e->y();

}

void GraphicsWidget::wheelEvent(QWheelEvent* e)
{
    float value = e->angleDelta().ry();
    value = value / abs(value);
    cameraDistance -= value * 0.5f;
    if (cameraDistance < 1.0f) cameraDistance = 1.0f;
    updateMatrix(width(), height());
}

void GraphicsWidget::stepEvent()
{
    auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    currentTime = millis / 1000.0f;
    repaint();
}
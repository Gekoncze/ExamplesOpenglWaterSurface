#pragma once
#include <GL/glew.h>
#include <qopenglwidget.h>
#include <qtimer.h>

class GraphicsWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    QTimer timer;
    bool drag = false;
    int dragX = 0;
    int dragY = 0;
    int moveX = 0;
    int moveY = 0;
    float dragCamYaw = 0;
    float dragCamPitch = 0;


public:
    GraphicsWidget(QWidget* parent);
    ~GraphicsWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);

private:
    void rotateCamera();

private slots:
    void stepEvent();
};


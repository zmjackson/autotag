#include "dataviewwidget3d.h"

DataViewWidget3D::DataViewWidget3D(QWidget *parent)
    : QOpenGLWidget(parent),
      xRot(0),
      yRot(0),
      zRot(0),
      shaderProgram(nullptr),
      currentDataFrame("test.ply") {}

DataViewWidget3D::~DataViewWidget3D()
{
    if (shaderProgram == nullptr)
        return;
    makeCurrent();
    vbo.destroy();
    delete shaderProgram;
    shaderProgram = 0;
    doneCurrent();
}

QSize DataViewWidget3D::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize DataViewWidget3D::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void DataViewWidget3D::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void DataViewWidget3D::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void DataViewWidget3D::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void DataViewWidget3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    shaderProgram = new QOpenGLShaderProgram;
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.vert");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragment.frag");
    shaderProgram->bindAttributeLocation("vertex", 0);
    shaderProgram->link();

    shaderProgram->bind();
    projMatrixLoc = shaderProgram->uniformLocation("projMatrix");
    mvMatrixLoc = shaderProgram->uniformLocation("mvMatrix");
    normalMatrixLoc = shaderProgram->uniformLocation("normalMatrix");

    vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);

    vbo.create();
    vbo.bind();
    vbo.allocate(currentDataFrame.vertices.data(), currentDataFrame.vertices.size() * sizeof(float));

    setupVertexAttribs();

    camera.setToIdentity();
    camera.translate(0, 0, -1);

    shaderProgram->release();
}

void DataViewWidget3D::setupVertexAttribs()
{
    vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    vbo.release();
}

void DataViewWidget3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    world.setToIdentity();
    world.rotate(180.0f - (xRot / 16.0f), 1, 0, 0);
    world.rotate(yRot / 16.0f, 0, 1, 0);
    world.rotate(zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    shaderProgram->bind();
    shaderProgram->setUniformValue(projMatrixLoc, proj);
    shaderProgram->setUniformValue(mvMatrixLoc, camera * world);
    QMatrix3x3 normalMatrix = world.normalMatrix();
    shaderProgram->setUniformValue(normalMatrixLoc, normalMatrix);

    glDrawArrays(GL_POINTS, 0, currentDataFrame.vertices.size() / 3);

    shaderProgram->release();
}

void DataViewWidget3D::resizeGL(int width, int height)
{
    proj.setToIdentity();
    proj.perspective(45.0f, GLfloat(width) / height, 0.01f, 100.0f);
}

void DataViewWidget3D::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void DataViewWidget3D::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

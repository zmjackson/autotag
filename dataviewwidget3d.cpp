#include "dataviewwidget3d.h"

DataViewWidget3D::DataViewWidget3D(QWidget *parent)
    : QOpenGLWidget(parent),
      xRot(0),
      yRot(0),
      zRot(0),
      m_fov(45.0f),
      dataShaderProgram(nullptr),
      m_currentDataFrame() {}

DataViewWidget3D::~DataViewWidget3D()
{
    if (dataShaderProgram == nullptr)
        return;
    makeCurrent();
    dataVbo.destroy();
    delete dataShaderProgram;
    dataShaderProgram = 0;
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

void DataViewWidget3D::changeCurrentDataFrame(const DataFrame3D &frame)
{
    m_currentDataFrame = frame;    
    dataVbo.bind();
    dataVbo.write(0, m_currentDataFrame.pointData().constData(), m_currentDataFrame.numDataPoints() * 4 * sizeof(float));
    dataVbo.release();
    update();
}

void DataViewWidget3D::setMaxDataPoints(const int newMaxDataPoints)
{    
    dataVbo.bind();
    dataVbo.allocate(m_currentDataFrame.pointData().constData(), newMaxDataPoints * sizeof(float));
    dataVbo.release();
}

void DataViewWidget3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    dataShaderProgram = new QOpenGLShaderProgram;
    dataShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "datapoint.vert");
    dataShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "datapoint.frag");
    dataShaderProgram->bindAttributeLocation("vertex", 0);
    dataShaderProgram->link();

    dataShaderProgram->bind();
    projMatrixLoc = dataShaderProgram->uniformLocation("projMatrix");
    mvMatrixLoc = dataShaderProgram->uniformLocation("mvMatrix");
    normalMatrixLoc = dataShaderProgram->uniformLocation("normalMatrix");

    dataVao.create();
    QOpenGLVertexArrayObject::Binder dataVaoBinder(&dataVao);

    dataVbo.create();
    dataVbo.bind();
    dataVbo.allocate((100000 * 4 * sizeof(float)));

    setupVertexAttribs();

    camera.setToIdentity();
    camera.translate(0, 0, -1);

    dataShaderProgram->release();
}

void DataViewWidget3D::setupVertexAttribs()
{
    dataVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    dataVbo.release();
}

void DataViewWidget3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    world.setToIdentity();
    world.rotate(180.0f - (xRot / 16.0f), 1, 0, 0);
    world.rotate(yRot / 16.0f, 0, 1, 0);
    world.rotate(zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder dataVaoBinder(&dataVao);
    dataShaderProgram->bind();
    float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());
    proj.setToIdentity();
    proj.perspective(m_fov, aspectRatio, 0.1f, 1000.0f);
    dataShaderProgram->setUniformValue(projMatrixLoc, proj);
    dataShaderProgram->setUniformValue(mvMatrixLoc, camera * world);
    QMatrix3x3 normalMatrix = world.normalMatrix();
    dataShaderProgram->setUniformValue(normalMatrixLoc, normalMatrix);

    glDrawArrays(GL_POINTS, 0, m_currentDataFrame.numDataPoints());

    dataShaderProgram->release();
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

    if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void DataViewWidget3D::wheelEvent(QWheelEvent *event)
{
    QPoint degrees = event->angleDelta() / 8;

    if (m_fov >= 1.0f && m_fov <= 45.0f)
        m_fov -= degrees.y();
    if (m_fov <= 1.0f)
        m_fov = 1.0f;
    if (m_fov >= 45.0f)
        m_fov = 45.0f;

    update();
}

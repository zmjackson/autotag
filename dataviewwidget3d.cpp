#include "dataviewwidget3d.h"

DataViewWidget3D::DataViewWidget3D(QWidget *parent)
    : QOpenGLWidget(parent),
      xRot(0),
      yRot(0),
      zRot(0),
      m_fov(45.0f),
      dataVbo(QOpenGLBuffer::VertexBuffer),
      trackingVbo(QOpenGLBuffer::VertexBuffer),
      trackingEbo(QOpenGLBuffer::IndexBuffer),
      dataShaderProgram(nullptr),
      trackingShaderProgram(nullptr),
      m_currentDataFrame(),
      classColors({{"VEHICLE", {1.0f, 0.30f, .80f, 1.0f}}, {"ON_ROAD_OBSTACLE", {1.0f, 1.0f, 0.0f, 1.0f}}, {"PEDESTRIAN", {0.0f, 1.0f, 0.0f, 1.0f}}, {"TRUCK", {0.0f, 0.0f, 1.0f, 1.0f}}})
{}

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
    glClearColor(0.1, 0.1, 0.1, 1);

    createDataShaderProgram();
    createTrackingShaderProgram();

    createDataGl();
    setupDataVertexAttribs();

    createTrackingGl();
    setupTrackingVertexAttribs();

    view.setToIdentity();
    view.translate(0, 0, -1);

    dataShaderProgram->release();
}

void DataViewWidget3D::createDataShaderProgram()
{
    dataShaderProgram = new QOpenGLShaderProgram;
    dataShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "data.vert");
    dataShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "data.frag");
    dataShaderProgram->link();
    dataShaderProgram->bind();

    dataProjMatrixLoc = dataShaderProgram->uniformLocation("projection");
    dataViewMatrixLoc = dataShaderProgram->uniformLocation("view");
    dataModelMatrixLoc = dataShaderProgram->uniformLocation("model");
}

void DataViewWidget3D::createTrackingShaderProgram()
{
    trackingShaderProgram = new QOpenGLShaderProgram;
    trackingShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "tracking.vert");
    trackingShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "tracking.frag");    
    trackingShaderProgram->link();
    trackingShaderProgram->bind();

    trackingProjMatrixLoc = dataShaderProgram->uniformLocation("projection");
    trackingViewMatrixLoc = dataShaderProgram->uniformLocation("view");
    trackingModelMatrixLoc = dataShaderProgram->uniformLocation("model");
    classColorLoc = trackingShaderProgram->uniformLocation("classColor");
}

void DataViewWidget3D::createDataGl()
{
    dataVao.create();
    dataVao.bind();
    dataVbo.create();
    dataVbo.bind();
    dataVbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    dataVbo.allocate((100000 * 4 * sizeof(float)));
    dataVbo.release();
    dataVao.release();
}

void DataViewWidget3D::createTrackingGl()
{
    trackingVao.create();
    trackingVao.bind();
    trackingVbo.create();
    trackingVbo.bind();
    trackingVbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    trackingVbo.allocate(8 * 3 * sizeof(float));
    trackingEbo.create();
    trackingEbo.bind();
    trackingEbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // Front face counterclockwise from top right
        4, 5, 5, 6, 6, 7, 7, 4, // Back face counterclockwise from top right
        0, 4, 1, 5, 2, 6, 3, 7  // "Connections" counterclockwise from top right
    };
    trackingEbo.allocate(indices.constData(), indices.size() * sizeof(unsigned int));
    trackingEbo.release();
    trackingVao.release();
}

void DataViewWidget3D::setupDataVertexAttribs()
{
    dataVao.bind();
    dataVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    dataVbo.release();
    dataVao.release();
}

void DataViewWidget3D::setupTrackingVertexAttribs()
{
    trackingVao.bind();
    trackingVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    trackingVbo.release();
    trackingVao.release();
}

void DataViewWidget3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    dataModel.setToIdentity();
    dataModel.rotate(180.0f - (xRot / 16.0f), 1, 0, 0);
    dataModel.rotate(yRot / 16.0f, 0, 1, 0);
    dataModel.rotate(zRot / 16.0f, 0, 0, 1);

    float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());
    proj.setToIdentity();
    proj.perspective(m_fov, aspectRatio, 0.1f, 1000.0f);

    dataVao.bind();
    dataShaderProgram->bind();
    dataShaderProgram->setUniformValue(dataProjMatrixLoc, proj);
    dataShaderProgram->setUniformValue(dataViewMatrixLoc, view);
    dataShaderProgram->setUniformValue(dataModelMatrixLoc, dataModel);
    glDrawArrays(GL_POINTS, 0, m_currentDataFrame.numDataPoints());

    trackingVao.bind();
    trackingShaderProgram->bind();
    trackingShaderProgram->setUniformValue(trackingProjMatrixLoc, proj);
    trackingShaderProgram->setUniformValue(trackingViewMatrixLoc, view);
    trackingVbo.bind();
    int ts = m_currentDataFrame.trackingData().size();
    for (const TrackedObject &object : m_currentDataFrame.trackingData()) {
        trackingVbo.write(0, object.verts.constData(), 24 * sizeof(float));
        trackingShaderProgram->setUniformValue(trackingModelMatrixLoc, dataModel * QMatrix4x4(object.rotation));
        trackingShaderProgram->setUniformValue(classColorLoc, classColors[object.labelClass]);

        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, indices.constData());
    }
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

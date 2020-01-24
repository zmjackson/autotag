#ifndef DATAVIEWWIDGET3D_H
#define DATAVIEWWIDGET3D_H

#include <vector>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QPoint>
#include <unordered_map>

#include "dataframe3d.h"

class DataViewWidget3D : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    DataViewWidget3D(QWidget *parent);
    ~DataViewWidget3D();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void changeCurrentDataFrame(const DataFrame3D &frame);
    void setMaxDataPoints(const int maxDataPoints);
    //void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void createDataShaderProgram();
    void createTrackingShaderProgram();
    void setupDataVertexAttribs();
    void setupTrackingVertexAttribs();
    void createDataGl();
    void createTrackingGl();

    DataFrame3D m_currentDataFrame;
    unsigned long m_maxDataPoints;

    int xRot;
    int yRot;
    int zRot;
    float m_fov;
    QPoint lastPos;    
    QOpenGLVertexArrayObject dataVao;
    QOpenGLVertexArrayObject trackingVao;
    QOpenGLBuffer dataVbo;
    QOpenGLBuffer trackingVbo;
    QOpenGLBuffer trackingEbo;
    QOpenGLShaderProgram *dataShaderProgram;
    QOpenGLShaderProgram *trackingShaderProgram;
    int dataProjMatrixLoc;
    int dataViewMatrixLoc;
    int dataModelMatrixLoc;
    int trackingProjMatrixLoc;
    int trackingViewMatrixLoc;
    int trackingModelMatrixLoc;
    int classColorLoc;
    QMatrix4x4 proj;
    QMatrix4x4 view;
    QMatrix4x4 dataModel;
    QMatrix4x4 boxModel;

    QVector<unsigned int> indices;
    std::unordered_map<std::string, QVector4D> classColors;
};

#endif // DATAVIEWWIDGET3D_H

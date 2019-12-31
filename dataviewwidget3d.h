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

#include "dataframe3d.h"
#include "datatimeline.h"

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
    void changeCurrentFrame(const DataFrame3D &frame);
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

private:
    void setupVertexAttribs();

    DataFrame3D currentDataFrame;

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    //DataFrame3D dataFrame;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram *shaderProgram;
    int projMatrixLoc;
    int mvMatrixLoc;
    int normalMatrixLoc;
    int lightPosLoc;
    QMatrix4x4 proj;
    QMatrix4x4 camera;
    QMatrix4x4 world;
};

#endif // DATAVIEWWIDGET3D_H

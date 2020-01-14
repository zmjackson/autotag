#ifndef TRACKINGFRAME_H
#define TRACKINGFRAME_H

#include <QVector3D>
#include <Qstring>

class TrackingFrame
{
public:
    TrackingFrame();

private:
    QVector3D<float> center;
    QVector4D<float> rotation;
    float length;
    float width;
    float height;
    QString trackLabelUuid;
    int timeStamp; // what type?
    QString labelClass;
};

#endif // TRACKINGFRAME_H

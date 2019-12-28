#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QVector3D>

class DataPoint
{
public:
    DataPoint();
    DataPoint(QVector3D position, unsigned char intensity, unsigned short laserNumber);

private:
    QVector3D position;
    unsigned char intensity;
    unsigned short laserNumber;
};

#endif // DATAPOINT_H

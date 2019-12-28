#include "datapoint.h"

DataPoint::DataPoint() : position({0, 0, 0}), intensity(0), laserNumber(0) {}

DataPoint::DataPoint(QVector3D position, unsigned char intensity, unsigned short laserNumber) :
    position(position), intensity(intensity), laserNumber(laserNumber)
{

}

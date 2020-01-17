#ifndef DATAFRAME3D_H
#define DATAFRAME3D_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <QVector3D>
#include <QVector>

#include <algorithm>

#include "tinyply.h"
#include "datapoint.h"
#include "utilities.cpp"
#include "trackedobject.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

class DataFrame3D
{
public:
    DataFrame3D();
    DataFrame3D(const QString &filePath);
    unsigned long timeStamp() const;
    int numDataPoints() const;
    const QVector<float> &pointData() const;
    const TrackedObject loadJson(const QString fileName);

private:          
    std::vector<char> loadBinaryFile(const std::string &filePath);
    std::vector<DataPoint> parsePlyFile(const std::vector<char> &binaryData);

    unsigned long m_timeStamp;
    QVector<float> m_vertData;
    int m_numDataPoints;

    QVector<TrackedObject> m_trackingData;
};

#endif // DATAFRAME3D_H

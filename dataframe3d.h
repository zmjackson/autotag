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
    const QVector<float> &positions() const;
    const QVector<unsigned char> &intensities() const;
    const QVector<unsigned short> &laserNumbers() const;
    /*size_t positionsSizeBytes() const;
    size_t laserNumbersSizeBytes() const;
    size_t intensitiesSizeBytes() const;*/

private:          
    std::vector<char> loadBinaryFile(const std::string &filePath);
    std::vector<DataPoint> parsePlyFile(const std::vector<char> &binaryData);

    unsigned long m_timeStamp;
    QVector<float> m_positions;
    QVector<unsigned char> m_intensities;
    QVector<unsigned short> m_laserNumbers;
    int m_numDataPoints;
};

#endif // DATAFRAME3D_H

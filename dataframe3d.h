#ifndef DATAFRAME3D_H
#define DATAFRAME3D_H

#include <QVector>
#include <set>

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
    const QVector<TrackedObject> &trackingData() const;
    const std::set<std::string> loadJson(const QString fileName);
    void resizeTrackingData(int size);

private:          
    std::vector<char> loadBinaryFile(const std::string &filePath);
    std::vector<DataPoint> parsePlyFile(const std::vector<char> &binaryData);

    unsigned long m_timeStamp;
    QVector<float> m_vertData;
    int m_numDataPoints;
    int maxDistanceRef;

    QVector<TrackedObject> m_trackingData;
};

#endif // DATAFRAME3D_H

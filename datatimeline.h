#ifndef DATATIMELINE_H
#define DATATIMELINE_H

#include <vector>
#include "dataframe3d.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class DataTimeline
{
public:
    DataTimeline();
    DataTimeline(const QStringList &fileList);
    const DataFrame3D &currentFrame() const;
    const DataFrame3D &frameAt(const int timeStamp) const;
    void addFrame(DataFrame3D &&frame);
    void addTrackingData(const QStringList &files);
    void setCurrentFrame(const int timeStamp);
    void nextFrame();
    void prevFrame();
    int numFrames() const;
    void reset(const QStringList &fileList);
    int maxDataPoints() const;

private:    
    std::vector<DataFrame3D> m_dataFrames;
    int m_currentTimeStamp;
    int m_maxDataPoints;
};

#endif // DATATIMELINE_H

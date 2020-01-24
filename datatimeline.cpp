#include "datatimeline.h"
#include <QStringList>
#include <QString>

DataTimeline::DataTimeline()
{
    m_currentTimeStamp = 0;
}

DataTimeline::DataTimeline(const QStringList &fileList)
    : m_currentTimeStamp(0)
{    
    m_dataFrames.reserve(fileList.size());
    m_maxDataPoints = 0;
    auto it = m_dataFrames.begin();
    for (const QString &fileName : fileList) {
        m_dataFrames.emplace(it, DataFrame3D(fileName));
        if (it->numDataPoints() > m_maxDataPoints)
            m_maxDataPoints = it->numDataPoints();
        ++it;
    }
}

const DataFrame3D &DataTimeline::currentFrame() const
{
    return m_dataFrames[m_currentTimeStamp];
}

void DataTimeline::addFrame(DataFrame3D &&frame)
{
    m_dataFrames.push_back(std::forward<DataFrame3D>(frame));
}

void DataTimeline::addTrackingData(const QStringList &files)
{
    for (int i = 0; i < files.size(); ++i)
        m_dataFrames[i].loadJson(files[i]);
}

void DataTimeline::setCurrentFrame(const int timeStamp)
{
    m_currentTimeStamp = timeStamp;
}

void DataTimeline::nextFrame()
{
    int frames = this->numFrames();
    m_currentTimeStamp = (m_currentTimeStamp + 1) % frames;
}

void DataTimeline::prevFrame()
{
    int frames = this->numFrames();
    m_currentTimeStamp = ((m_currentTimeStamp - 1) + frames) % frames;
}

int DataTimeline::numFrames() const
{
    return m_dataFrames.size();
}

int DataTimeline::maxDataPoints() const
{
    return m_maxDataPoints;
}

#include "datatimeline.h"

DataTimeline::DataTimeline()
{

}

const DataFrame3D &DataTimeline::currentFrame() const
{
    return dataFrames.at(currentTimeStamp);
}

const DataFrame3D &DataTimeline::frameWithId(const std::string &id) const
{
    return dataFrames.at(timeStamps.at(id));
}

void DataTimeline::nextFrame()
{
    ++currentTimeStamp;
}

void DataTimeline::prevFrame()
{
    --currentTimeStamp;
}

void DataTimeline::numFrames()
{
    return dataFrames.size();
}

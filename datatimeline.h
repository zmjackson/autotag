#ifndef DATATIMELINE_H
#define DATATIMELINE_H

#include <map>

#include "dataframe3d.h"

class DataTimeline
{
public:
    DataTimeline();    
    const DataFrame3D &currentFrame() const;
    const DataFrame3D &frameWithId(const std::string &id) const;
    const DataFrame3D &frameAtTime(const unsigned long timeStamp) const;
    void setCurrentFrame(const std::string &id) const;
    void setCurrentFrame(const unsigned long timeStamp) const;
    void nextFrame();
    void prevFrame();
    void numFrames();

private:
    unsigned long currentTimeStamp;
    std::map<std::string, unsigned long> timeStamps;
    std::map<unsigned long, DataFrame3D> dataFrames;
};

#endif // DATATIMELINE_H

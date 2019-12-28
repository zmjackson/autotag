#ifndef DATATIMELINE_H
#define DATATIMELINE_H

#include <map>

#include "dataframe3d.h"

class DataTimeline
{
public:
    DataTimeline();
    std::unique_ptr<const DataFrame3D> currentFrame();

private:
    std::map<int, DataFrame3D> dataFrames;
};

#endif // DATATIMELINE_H

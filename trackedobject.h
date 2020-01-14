#ifndef TRACKEDOBJECT_H
#define TRACKEDOBJECT_H

#include <string>
#include <list>

#include <trackingframe.h>

class TrackedObject
{
public:
    TrackedObject();

private:
    std::string labeClass;
    std::string uuid;
    std::string logId;
    std::list<TrackingFrame> trackLabelFrames;
};

#endif // TRACKEDOBJECT_H

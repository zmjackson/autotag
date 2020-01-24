#ifndef TRACKEDOBJECT_H
#define TRACKEDOBJECT_H

#include <string>
#include <list>
#include <QMatrix4x4>

struct TrackedObject
{
    QVector<float> verts;
    QVector3D position;
    QMatrix3x3 rotation;
    std::string uuid;
    uint64_t timestamp = 0;
    std::string labelClass;
};

#endif // TRACKEDOBJECT_H

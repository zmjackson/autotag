#ifndef DATAFRAME3D_H
#define DATAFRAME3D_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <QVector3D>

#include <algorithm>

#include "tinyply.h"
#include "datapoint.h"
#include "utilities.cpp"

namespace autotag {
class DataFrame3D;
}

class DataFrame3D
{
public:
    DataFrame3D(const std::string &filePath);
    std::vector<float> vertices;

private:
    std::vector<char> loadBinaryFile(const std::string &filePath);
    std::vector<DataPoint> parsePlyFile(const std::vector<char> &binaryData);

    unsigned long timeStamp;
    std::vector<DataPoint> pointCloud;
};

#endif // DATAFRAME3D_H

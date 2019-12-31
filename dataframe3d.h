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
    std::string id() const;
    unsigned long timeStamp() const;
    const std::vector &positions() const;
    const std::vector &intensities() const;
    const std::vector &laserNumbers() const;
    int numDataPoints() const;

private:          
    std::vector<char> loadBinaryFile(const std::string &filePath);
    std::vector<DataPoint> parsePlyFile(const std::vector<char> &binaryData);

    std::string id;
    unsigned long timeStamp;
    std::vector<float> positions;
    std::vector<unsigned char> intensities;
    std::vector<unsigned short> laserNumbers;
    int numDataPoints;
};

#endif // DATAFRAME3D_H

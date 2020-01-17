#include "dataframe3d.h"
#include "json/json.h"
#include <QString>
#include <fstream>
#include <iostream>
#include <string>
#include <QVector4D>
#include <QQuaternion>


DataFrame3D::DataFrame3D()
    : m_timeStamp(0),
      m_numDataPoints(0) {}

DataFrame3D::DataFrame3D(const QString &filePath)
{
    //std::vector<char> file = loadBinaryFile(filePath);
    //pointCloud = parsePlyFile(file);
    std::ifstream fileStream(filePath.toStdString(), std::ios::binary);
    tinyply::PlyFile file;
    file.parse_header(fileStream);

    std::shared_ptr<tinyply::PlyData> positionData, intensityData, laserNumberData;
    positionData = file.request_properties_from_element("vertex", { "x", "y", "z" });
    intensityData = file.request_properties_from_element("vertex", { "intensity" });
    laserNumberData = file.request_properties_from_element("vertex", { "laser_number" });
    // Todo: Assertions?
    file.read(fileStream);
    //printPlyInfo(file);
    m_numDataPoints = positionData->count;

    std::vector<float> positions(3 * positionData->count);
    std::memcpy(positions.data(), positionData->buffer.get(), positionData->buffer.size_bytes());
    float max_val = *std::max_element(positions.begin(), positions.end());
    for (auto& it : positions)
        it /= max_val;

    std::vector<unsigned char> intensities(intensityData->count);
    std::memcpy(intensities.data(), intensityData->buffer.get(), intensityData->buffer.size_bytes());

    std::vector<unsigned short> laserNumbers(laserNumberData->count);
    std::memcpy(laserNumbers.data(), laserNumberData->buffer.get(), laserNumberData->buffer.size_bytes());

    m_vertData.resize(m_numDataPoints * 4);
    float ucharMax = static_cast<float>(std::numeric_limits<char>::max());
    int positionIndex = 0;
    int intensityIndex = 0;
    for (int vertIndex = 0; vertIndex < m_vertData.size(); vertIndex += 4) {
        m_vertData[vertIndex + 0] = positions[positionIndex++];
        m_vertData[vertIndex + 1] = positions[positionIndex++];
        m_vertData[vertIndex + 2] = positions[positionIndex++];
        // Normalize intesnsity to a float between 0 and 1
        m_vertData[vertIndex + 3] = static_cast<float>(intensities[intensityIndex++]) / ucharMax;
    }
}

/*std::vector<char> DataFrame3D::loadBinaryFile(const std::string &filePath)
{
    std::ifstream input(filePath, std::ios::binary | std::ios::ate);
    // Todo: error handling

    auto fileEnd = input.tellg();
    input.seekg(0, std::ios::beg);

    auto fileSize = std::size_t(fileEnd - input.tellg());

    std::vector<char> buffer(fileSize);
    input.read((char*)buffer.data(), buffer.size());

    return buffer;
}

std::vector<DataPoint> DataFrame3D::parsePlyFile(const std::vector<char> &binaryData)
{
    std::ifstream file_stream("test.ply", std::ios::binary);
    tinyply::PlyFile test_file;
    test_file.parse_header(file_stream);

    std::shared_ptr<tinyply::PlyData> position_data = test_file.request_properties_from_element("vertex", {"x", "y", "z"});
    test_file.read(file_stream);

    std::vector<float> test_positions(position_data->count * 3);
    std::memcpy(test_positions.data(), position_data->buffer.get(), position_data->buffer.size_bytes());



    tinyply::PlyFile file;
    CharVecBufferWrap fileBuffer(binaryData);
    std::istream fileStream(&fileBuffer);
    file.parse_header(fileStream);

    std::shared_ptr<tinyply::PlyData> positionData, intensityData, laserNumberData;

    positionData = file.request_properties_from_element("vertex", { "x", "y", "z" });
    intensityData = file.request_properties_from_element("vertex", { "intensity" });
    laserNumberData = file.request_properties_from_element("vertex", { "laser_number" });
    // Todo: Assertions?
    file.read(fileStream);
    //printPlyInfo(file);
    std::vector<float> positions(positionData->count * 3);
    std::memcpy(positions.data(), positionData->buffer.get(), positionData->buffer.size_bytes());
    std::vector<unsigned char> intensities(intensityData->count);
    std::memcpy(intensities.data(), intensityData->buffer.get(), intensityData->buffer.size_bytes());
    std::vector<unsigned short> laserNumbers(laserNumberData->count);
    std::memcpy(laserNumbers.data(), laserNumberData->buffer.get(), laserNumberData->buffer.size_bytes());

    for (const float& position : positions) std::cout << position << " ";

    std::vector<DataPoint> data(intensities.size());
    int posi = 0;
    int i = 0;
    for (auto &currDataPoint : data) {
        currDataPoint = { QVector3D(positions[posi], positions[posi + 1], positions[posi +2 ]),
                          intensities[i],
                          laserNumbers[i] };
        posi += 3;
        ++i;
     }

    return data;
}*/

unsigned long DataFrame3D::timeStamp() const
{
    return m_timeStamp;
}

int DataFrame3D::numDataPoints() const
{
    return m_numDataPoints;
}

const QVector<float> &DataFrame3D::pointData() const
{
    return m_vertData;
}

const TrackedObject DataFrame3D::loadJson(const QString fileName)
{
    std::ifstream file(fileName.toStdString(), std::ifstream::binary);
    file.seekg(0, file.end);
    int fileLength = file.tellg();
    file.seekg(0, file.beg);

    std::vector<char> buffer(fileLength);

    file.read(buffer.data(), fileLength);

    Json::Value scene;
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    std::string errors;

    bool parseSuccessful = reader->parse(buffer.data(), buffer.data() + fileLength, &scene, &errors);
    if (!parseSuccessful)
        std::cout << errors << "\n";

    m_trackingData.resize(scene.size());
    int i = 0;

    for(Json::Value object : scene) {
        TrackedObject annotation;
        float x, y, z, length, width, height;

        x = object["center"].get("x", 0).asFloat();
        y = object["center"].get("y", 0).asFloat();
        z = object["center"].get("z", 0).asFloat();

        length = object["length"].asFloat();
        width = object["width"].asFloat();
        height = object["height"].asFloat();

        // This assumes that length = x, width = y, height = z (may need to be changed)
        QVector<float> verts = {
            x + length/2, y + width/2, z + height/2, // Front right top
            x + length/2, y + width/2, z - height/2, // Front right bottom
            x + length/2, y - width/2, z - height/2, // Front left bottom
            x + length/2, y - width/2, z + height/2, // Front left top
            x - length/2, y + width/2, z + height/2, // Back right top
            x - length/2, y + width/2, z - height/2, // Back right bottom
            x - length/2, y - width/2, z - height/2, // Back left bottom
            x - length/2, y - width/2, z + height/2, // Back left top
        };

        QQuaternion rotationQuat;
        rotationQuat.setScalar(object["rotation"].get("w", 0).asFloat());
        rotationQuat.setX(object["rotation"].get("x", 0).asFloat());
        rotationQuat.setY(object["rotation"].get("y", 0).asFloat());
        rotationQuat.setZ(object["rotation"].get("z", 0).asFloat());
        QMatrix3x3 rotation = rotationQuat.toRotationMatrix();

        std::string uuid = object["track_label_uuid"].asString();
        uint64_t timestamp = object["timestamp"].asUInt64();
        std::string labelClass = object["label_class"].asString();

        return {verts, rotation, uuid, timestamp, labelClass};
    }
}

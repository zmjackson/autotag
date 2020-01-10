#include "dataframe3d.h"
#include <QString>

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

    m_positions.resize(3 * positionData->count);
    std::memcpy(m_positions.data(), positionData->buffer.get(), positionData->buffer.size_bytes());
    m_intensities.resize(intensityData->count);
    std::memcpy(m_intensities.data(), intensityData->buffer.get(), intensityData->buffer.size_bytes());
    m_laserNumbers.resize(laserNumberData->count);
    std::memcpy(m_laserNumbers.data(), laserNumberData->buffer.get(), laserNumberData->buffer.size_bytes());

    auto max = std::max_element(m_positions.begin(), m_positions.end());
    float max_val = *max;
    for (auto& it : m_positions)
        it /= max_val;    
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

const QVector<float> &DataFrame3D::positions() const
{
    return m_positions;
}

const QVector<unsigned char> &DataFrame3D::intensities() const
{
    return m_intensities;
}

const QVector<unsigned short> &DataFrame3D::laserNumbers() const
{
    return m_laserNumbers;
}


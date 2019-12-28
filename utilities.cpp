#include <streambuf>
#include <vector>

struct CharVecBufferWrap : std::streambuf
{
    CharVecBufferWrap(const std::vector<char> &vec)
    {
        char *data = const_cast<char*>(vec.data());
        this->setg(data, data, data + vec.size());
    }
};

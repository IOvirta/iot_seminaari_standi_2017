#ifndef IOVIRTA_IOT_VIDEO_ENCODER_ENCODED_FRAME_HPP
#define IOVIRTA_IOT_VIDEO_ENCODER_ENCODED_FRAME_HPP

#include <cstdint>
#include <cstddef>
#include <cstring>

#include <algorithm>

namespace iovirta_iot
{
namespace video
{
struct EncodedFrame
{
    std::uint8_t *data = nullptr;
    std::size_t size = 0;
    /*video size, koodaus*/

    EncodedFrame() {}
    ~EncodedFrame()
    {
        if (data != nullptr)
            delete[] data;
    }
    EncodedFrame(const EncodedFrame &other)
    {
        if (other.data != nullptr)
        {
            size = other.size;
            data = new std::uint8_t[size];

            std::memcpy(data, other.data, size);
        }
    }
    EncodedFrame(EncodedFrame &&other) : data(other.data), size(other.size)
    {
        other.size = 0;
        other.data = nullptr;
    }
    EncodedFrame& operator=(EncodedFrame other)
    {
        if (this != &other)
        {
            std::swap(data, other.data);
            std::swap(size, other.size);
        }

        return *this;
    }
}; // struct EncodedFrame
} // namespace video
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_VIDEO_ENCODER_ENCODED_FRAME_HPP

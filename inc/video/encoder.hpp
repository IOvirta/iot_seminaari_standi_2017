#ifndef IOVIRTA_IOT_VIDEO_ENCODER_ENCODER_HPP
#define IOVIRTA_IOT_VIDEO_ENCODER_ENCODER_HPP

#include <string>

#include "video/encoded_frame.hpp"

namespace iovirta_iot
{
namespace video
{
class Encoder
{
public:
    //Encoder(const std::string &codec) {}
    Encoder(const std::string &codec, int width, int height) {}
    ~Encoder() {}
    bool open_file(const std::string &file) {}
    bool close_file() {}
    EncodedFrame encode_frame() {}
    //EncodedFrame encode_frame(/**/);
}; // class Encoder
} // namespace video
} // namespae iovirta_iot

#endif // IOVIRTA_IOT_VIDEO_ENCODER_ENCODER_HPP

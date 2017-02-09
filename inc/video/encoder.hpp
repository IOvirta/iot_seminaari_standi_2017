#ifndef IOVIRTA_IOT_VIDEO_ENCODER_ENCODER_HPP
#define IOVIRTA_IOT_VIDEO_ENCODER_ENCODER_HPP

#include <string>

#include <cstdint>
#include <cstdio>

extern "C" {
    #include <libavcodec/avcodec.h>
}

#include "video/encoded_frame.hpp"

namespace iovirta_iot
{
namespace video
{

class Encoder
{
private:
    std::FILE *output_file_ = nullptr;
    std::string codec_;
    int width_;
    int height_;

    bool init();
public:
    //Encoder(const std::string &codec) {}
    Encoder(const std::string &codec, int width, int height) : codec_(codec), width_(width), height_(height) {}
    ~Encoder() { close_file(); }

    static void register_codecs()
    {
        {
            static bool done = false;
            if (!done)
                avcodec_register_all();

            done = true;
        }
    }

    bool open_file(const std::string &file);
    bool close_file();
    EncodedFrame encode_frame() {return {};}
}; // class Encoder
} // namespace video
} // namespae iovirta_iot

#endif // IOVIRTA_IOT_VIDEO_ENCODER_ENCODER_HPP

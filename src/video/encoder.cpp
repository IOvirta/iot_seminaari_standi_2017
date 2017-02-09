#include <string>
#include <cstdio>

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "video/encoder.hpp"
#include "video/encoded_frame.hpp"

using namespace iovirta_iot;

bool video::Encoder::init()
{
    // TODO

    return true;
}

bool video::Encoder::open_file(const std::string &file_name)
{
    close_file();

    output_file_ = std::fopen(file_name.c_str(), "wb");

    if (!output_file_)
        return false;

    return true;
}

bool video::Encoder::close_file()
{
    if (output_file_ != nullptr)
        std::fclose(output_file_);

    output_file_ = nullptr;

    return true;
}

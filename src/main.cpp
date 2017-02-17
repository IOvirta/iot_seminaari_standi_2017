#include <iostream>

#include <cstdio>

#include "conf/conf.hpp"
#include "video/camera.hpp"
#include "network/fcm_server.hpp"


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " conf.ini" << std::endl;
        return 0;
    }

    iovirta_iot::conf::Configuration conf(argv[1]);

    iovirta_iot::video::Camera camera(conf.get<int>("camera0", "id"),
        conf.get<int>("camera0", "width"), conf.get<int>("camera0", "height"));


    iovirta_iot::network::FCMServer fcm_server; // TODO oisko tää jonku muun niminen oikeesti

    const char *ffmpeg_command_fmt = "ffmpeg -f rawvideo -pixel_format bgr24 -video_size %dx%d -i pipe:0 -codec:v %s -pix_fmt %s -vprofile baseline -an -f flv %s";
    int ffmpeg_command_len = std::snprintf(nullptr, 0, ffmpeg_command_fmt, camera.width(), camera.height(),
        conf.get<std::string>("video", "codec").c_str(), conf.get<std::string>("video", "pix_fmt").c_str(), conf.get<std::string>("video", "output").c_str());

    std::vector<char> ffmpeg_command_buf(ffmpeg_command_len + 1);
    std::snprintf(&ffmpeg_command_buf[0], ffmpeg_command_buf.size(), ffmpeg_command_fmt, camera.width(), camera.height(),
        conf.get<std::string>("video", "codec").c_str(), conf.get<std::string>("video", "pix_fmt").c_str(), conf.get<std::string>("video", "output").c_str());

    std::FILE *ffmpeg = popen(std::string(ffmpeg_command_buf.begin(), ffmpeg_command_buf.end()).c_str(), "w");
    if (ffmpeg == nullptr)
    {
        std::cerr << "ffmpeg popen" << std::endl;
        return 1;
    }

    bool movement = false;

    int frame_size = camera.width() * camera.height() * 3;
    //while (true)
    for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        if (!camera.capture_frame())
        { /* ei saatu kuvaa */ }

        if (camera.check_motion())
        {
            if (!movement)
            {
                // liike alkoi, ilmotus puhelimelle
                movement = true;
            }
        }
        else
        {
            if (movement)
            {
                // liike loppui
                movement = false;
            }
        }

        fwrite(camera.frame_.data, sizeof(unsigned char), frame_size, ffmpeg);
    }

    pclose(ffmpeg);

    return 0;
}

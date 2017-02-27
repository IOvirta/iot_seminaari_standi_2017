#include <iostream>
#include <vector>
#include <chrono>
#include <cstdio>

#include "inotify++.hpp"

#include "conf/conf.hpp"
#include "video/camera.hpp"
#include "network/fcm_server.hpp"


void check_file_events(int ifd, pollfd &pfd);

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

    //std::FILE *ffmpeg = popen(std::string(ffmpeg_command_buf.begin(), ffmpeg_command_buf.end()).c_str(), "w");
    //if (ffmpeg == nullptr)
    //{
    //    std::cerr << "ffmpeg popen" << std::endl;
    //    return 1;
    //}
    /*std::FILE *video_recoder = popen(conf.get<std::string>("locations", "recorder").c_str(), "w"); // onks tääki tyhmä nimi
    if (video_recoder == nullptr)
    {
        std::cerr << ".....todo" << std::endl;
    }*/
    inotify::Inotify inotify;
    //inotify.add_watch(conf.get<std::string>("locations", "tokens"), IN_MODIFY);
    //inotify.add_watch(conf.get<std::string>("locations", "hls"), IN_CREATE);


    bool movement = false;
    cv::namedWindow("frame");
    int frame_size = camera.width() * camera.height() * 3;
    long long sum = 0;
    //while (true)
    for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        if (!camera.capture_frame())
        { /* ei saatu kuvaa */ break;}


        if (camera.check_motion())
        {
            if (!movement)
            {
                // liike alkoi, ilmotus puhelimelle
                movement = true;
                std::cout << "alkoi" << std::endl;
            }
        }
        else
        {
            if (movement)
            {
                // liike loppui
                std::cout << "loppui" << std::endl;
                movement = false;
            }
        }
        //fwrite(camera.frame_.data, sizeof(unsigned char), frame_size, ffmpeg);

        if (inotify.poll_events())
        {
            inotify::Event event;
            while (inotify.get_event(&event))
            {
                if (event.mask & IN_MODIFY)
                    ; // uus puhelin tullu, lisätään se johonki ylös että saadaan sinnekki ilmotukset lähetttyä
                if (event.mask & IN_CREATE)
                    ; // uus videon pätkä tullu, nimi ylös niin osataan sitten joskus tallentaa oikee video
            }
        }
    }
    //pclose(ffmpeg);

    return 0;
}

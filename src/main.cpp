#include <iostream>
#include <atomic>
#include <chrono>


#include <opencv2/highgui/highgui.hpp> // tulee poistumaan

#include "conf/conf.hpp"
#include "video/camera.hpp"
#include "network/video_streamer.hpp"
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


    iovirta_iot::network::VideoStreamer streamer(conf.get<int>("iv", "port"));
    streamer.start();

    iovirta_iot::network::FCMServer fcm_server/*()
    std::thread ....., std::ref(flags));*/;

    // v--------- aikanaan poistuu
    if (conf.get<bool>("test", "nayta"))
        cv::namedWindow("frame");

    bool movement = false;
    std::chrono::time_point<std::chrono::steady_clock> movement_stop_time;
    // while (flags & run)
    //for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    while(true)
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

        streamer.send(camera.frame_);

        // v---- aikanaan poistuu
        if (conf.get<bool>("test", "nayta"))
            cv::imshow("frame", camera.frame_);

        if (cv::waitKey(1) == 'q')
            break;
    }

    streamer.stop();

    return 0;
}

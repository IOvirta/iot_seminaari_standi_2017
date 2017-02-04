#include <iostream>


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "conf/conf.hpp"
#include "video/camera.hpp"
//#include "video/motion.hpp"

//#include

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " conf.ini" << std::endl;
        return 0;
    }

    iovirta_iot::conf::Configuration conf(argv[1]);

    iovirta_iot::video::Camera camera;
    if (!iovirta_iot::video::open_camera(conf.get<int>("camera0", "id"), camera))
    {
        return 1;
    }

    /* kameran asetukset */
    camera.set_width(conf.get<int>("camera0", "width"));
    camera.set_height(conf.get<int>("camera0", "height"));

    // v--------- aikanaan poistuu
    if (conf.get<bool>("test", "nayta"))
        cv::namedWindow("frame");

    // while (?)
    for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        if (!iovirta_iot::video::capture_frame(camera))
        { /* ei saatu kuvaa */ }

        //if (iovirta_iot::video::check_motion(camera))
        {
            /*  jos edellisellä framella ei ollut liikettä lähetetään puhelimelle
                ilmotus ja avataan tiedosto tallentamista vartern
                encoder.open_file(xxx.zzz);
            */
        }
        /*
        else
        {
            jos viime framella oli liikettä pistetään kello pyöriin
            muuten katotaan onko kello pöyriny x aikaa ja jos on
            niin pistetää tiedosto kii
            enoder.close_file();
        }

        if (puhelin pyytänyt kuvaa || kamerassa liikettä)
        {
            xxx = encoder.encode_frame(camera.frame);

            if (pyydetty puhelimelta)
                video_streamer.send_frame(camera.frame);
        }
        */


        // v---- aikanaan poistuu
        if (conf.get<bool>("test", "nayta"))
            cv::imshow("frame", camera.frame);

        cv::waitKey(1);
    }

    return 0;
}

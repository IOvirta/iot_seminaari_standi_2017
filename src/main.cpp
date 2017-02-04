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
    
    std::vector<iovirta_iot::video::Camera> cameras;

    for (auto &i : conf.get<std::vector<std::string>>("camera"))
    {
        auto id = conf.get<int>(i, "id");
        iovirta_iot::video::Camera cam;
        if (iovirta_iot::video::open_camera(id, cam))
            cameras.push_back(cam);
        else
            std::cerr << "kameraa camera" << id << " ei saatu auki" << std::endl;
    }


    /* kameroiden asetukset */
    for (auto &cam : cameras)
    {
        auto cam_str = "camera" + std::to_string(cam.id);
        cam.set_width(conf.get<int>(cam_str, "width"));
        cam.set_height(conf.get<int>(cam_str, "height"));
    }


    if (conf.get<bool>("test", "nayta"))
        cv::namedWindow("frame");

    // while (käynnissä olo ehto)
    for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        for (auto &cam : cameras)
            if (!iovirta_iot::video::capture_frame(cam))
                { /*TODO*/}



        if (conf.get<bool>("test", "nayta"))
            cv::imshow("frame", cameras[0].frame);

        cv::waitKey(1);
    }

    return 0;
}

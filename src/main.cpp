#include <iostream>


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "conf/conf.hpp"


//#include

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " conf.ini" << std::endl;
        return 0;
    }

    iovirta_iot::conf::Configuration conf(argv[1]);


    cv::VideoCapture video_in(0);

    video_in.set(CV_CAP_PROP_FRAME_WIDTH, conf.get<int>("video", "width"));
    video_in.set(CV_CAP_PROP_FRAME_HEIGHT, conf.get<int>("video", "height"));

    if (!video_in.isOpened())
    {
        // log.error(...);
        std::cout << "!video_in.isOpened()" << std::endl;
        return 0;
    }

    cv::Mat frame;
    video_in >> frame;

    if (conf.get<bool>("test", "nayta"))
        cv::namedWindow("frame");

    // while (käynnissä olo ehto)
    for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        video_in >> frame;


        if (conf.get<bool>("test", "nayta"))
            cv::imshow("frame", frame);

        cv::waitKey(1);
    }

    return 0;
}

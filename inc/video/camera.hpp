#ifndef IOVIRTA_IOT_VIDEO_CAMERA_HPP
#define IOVIRTA_IOT_VIDEO_CAMERA_HPP

#include <vector>

#include <opencv2/opencv.hpp>

namespace iovirta_iot
{
namespace video
{
struct Camera
{
    cv::VideoCapture cap;
    cv::Mat frame;

    int id = -1;

    void set_width(int width)
    {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    }

    void set_height(int height)
    {
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }

    int get_width() { return cap.get(CV_CAP_PROP_FRAME_WIDTH); }
    int get_height() { return cap.get(CV_CAP_PROP_FRAME_HEIGHT); }
}; // struct Camera

bool open_camera(int id, Camera &cam);
bool capture_frame(Camera &cam);

} // namespace video
} // namespace iovirta_iot

#endif //IOVIRTA_IOT_VIDEO_CAMERA_HPP

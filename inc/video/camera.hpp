#ifndef IOVIRTA_IOT_VIDEO_CAMERA_HPP
#define IOVIRTA_IOT_VIDEO_CAMERA_HPP

#include <opencv2/opencv.hpp>

namespace iovirta_iot
{
namespace video
{
class Camera
{
private:
    // cv::Mat frame_;
    cv::VideoCapture video_in;

    int width_;
    int height_;
    int frame_count_ = 0;
public:
    cv::Mat frame_;
    Camera(int camera_id, int width, int height) : video_in(0), width_(width), height_(height)
    {
        if (!video_in.isOpened())
            throw std::runtime_error("todo");
    };
    ~Camera()
    {
    }
    bool capture_frame();
    bool check_motion();
}; // class Camera
} // namespace video
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_VIDEO_CAMERA_HPP

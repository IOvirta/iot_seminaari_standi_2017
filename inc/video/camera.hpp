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
    cv::Mat prev_gray_;

    int width_;
    int height_;
    int frame_count_ = 0;
public:
    cv::Mat frame_;
    Camera(int camera_id, int width, int height) : video_in(camera_id)
    {
        if (!video_in.isOpened())
            throw std::runtime_error("todo");

        video_in.set(CV_CAP_PROP_FRAME_WIDTH, width);
        video_in.set(CV_CAP_PROP_FRAME_HEIGHT, height);
        width_ = video_in.get(CV_CAP_PROP_FRAME_WIDTH);
        height_ = video_in.get(CV_CAP_PROP_FRAME_HEIGHT);

        prev_gray_ = cv::Mat(height_, width_, CV_8U);
    };
    ~Camera()
    {
    }
    bool capture_frame();
    bool check_motion();
    int width() { return width_; }
    int height() { return height_; }
}; // class Camera
} // namespace video
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_VIDEO_CAMERA_HPP

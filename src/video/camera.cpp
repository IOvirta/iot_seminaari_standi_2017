#include <opencv2/opencv.hpp>

#include "video/camera.hpp"


bool iovirta_iot::video::Camera::capture_frame()
{
    return video_in.read(frame_);
}

bool iovirta_iot::video::Camera::check_motion()
{
    return false;
}

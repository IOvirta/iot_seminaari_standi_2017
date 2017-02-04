#ifndef IOVIRTA_IOT_VIDEO_MOTION_HPP
#define IOVIRTA_IOT_VIDEO_MOTION_HPP

#include <vector>

#include <opencv2/opencv.hpp>

#include "video/camera.hpp"

namespace iovirta_iot
{
namespace video
{
bool check_motion(Camera &cam);
} // namespace video
} // namespace iovirta_iot

#endif //IOVIRTA_IOT_VIDEO_MOTION_HPP

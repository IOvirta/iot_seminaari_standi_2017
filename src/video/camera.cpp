#include "video/camera.hpp"

using namespace iovirta_iot;

bool video::open_camera(int id, Camera &cam)
{
    auto ret = cam.cap.open(id);
    if (ret)
        cam.id = id;

    return ret;
}

bool video::capture_frame(Camera &cam)
{
    return cam.cap.read(cam.frame);
}

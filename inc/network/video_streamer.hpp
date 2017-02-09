#ifndef IOVIRTA_IOT_NETWORK_VIDEO_STREAMER_HPP
#define IOVIRTA_IOT_NETWORK_VIDEO_STREAMER_HPP

namespace iovirta_iot
{
namespace network
{
class VideoStreamer
{
public:
    VideoStreamer() {}
    ~VideoStreamer() {}
    VideoStreamer(const VideoStreamer&) = delete;
    VideoStreamer(VideoStreamer &&) = delete;
}; // class VideoStreamer
} // namespace network
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_NETWORK_VIDEO_STREAMER_HPP

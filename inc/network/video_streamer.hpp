#ifndef IOVIRTA_IOT_NETWORK_VIDEO_STREAMER_HPP
#define IOVIRTA_IOT_NETWORK_VIDEO_STREAMER_HPP

#include <list>
#include <thread>
#include <utility>
#include <memory>

#include <asio.hpp>

#include <opencv2/opencv.hpp>

namespace iovirta_iot
{
namespace network
{
class VideoStreamer
{
private:
    asio::io_service io_service_;
    asio::ip::tcp::acceptor acceptor_;
    std::list<std::shared_ptr<asio::ip::tcp::socket>> connections_;
    std::thread io_service_thread_;
    int port_;

    void start_accept();
    void write_handler(std::list<std::shared_ptr<asio::ip::tcp::socket>>::iterator it, std::error_code error);
    void accept_handler(std::shared_ptr<asio::ip::tcp::socket> socket, std::error_code error);
public:
    VideoStreamer(int port) : io_service_(), acceptor_(io_service_), connections_(), port_(port) {}
    ~VideoStreamer() {}
    VideoStreamer(const VideoStreamer&) = delete;
    VideoStreamer(VideoStreamer &&) = delete;
    
    void send(cv::Mat frame);
    void start();
    void stop();
}; // class VideoStreamer
} // namespace network
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_NETWORK_VIDEO_STREAMER_HPP

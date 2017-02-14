#include <functional>
#include <thread>
#include <cstring>

#include <asio.hpp>

#include <opencv2/opencv.hpp>

#include "network/video_streamer.hpp"

void iovirta_iot::network::VideoStreamer::start()
{
    auto endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();

    io_service_thread_ = std::thread([](asio::io_service &io) { io.run(); }, std::ref(io_service_));
}

void iovirta_iot::network::VideoStreamer::stop()
{
    io_service_.stop();
    io_service_thread_.join();
}

void iovirta_iot::network::VideoStreamer::start_accept()
{
    auto socket = std::make_shared<asio::ip::tcp::socket>(asio::ip::tcp::socket(io_service_));
    auto handler = std::bind(&VideoStreamer::accept_handler, this, socket, std::placeholders::_1);
    acceptor_.async_accept(*socket, handler);
}

void iovirta_iot::network::VideoStreamer::send(cv::Mat frame)
{
    unsigned char *raw_frame = new unsigned char[frame.rows*frame.cols*3]; // TODO
    unsigned char *p = raw_frame;

    for (int j = 0; j < frame.rows; ++j)
    {
        std::memcpy(p, frame.ptr(j), sizeof(unsigned char) * frame.cols * 3);
        p += frame.cols * 3;
    }

    std::vector<unsigned char> buf(raw_frame, raw_frame + frame.cols * frame.rows * 3);

    for (auto it = connections_.begin(); it != connections_.end(); ++it)
    {
        auto handler = std::bind(&VideoStreamer::write_handler, this, it, std::placeholders::_1);
        asio::write(*(*it), asio::buffer(buf));
    }

    delete[] raw_frame;
}

void iovirta_iot::network::VideoStreamer::accept_handler(std::shared_ptr<asio::ip::tcp::socket> socket, std::error_code error)
{
    if (!error)
        connections_.emplace_back(socket);
}

void iovirta_iot::network::VideoStreamer::write_handler(std::list<std::shared_ptr<asio::ip::tcp::socket>>::iterator it, std::error_code error)
{
    if (error)
        connections_.erase(it);
}

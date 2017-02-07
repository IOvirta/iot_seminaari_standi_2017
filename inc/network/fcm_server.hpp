#ifndef IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP
#define IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP

#include <iostream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include <sstream>
#include <asio/ssl.hpp>

namespace iovirta_iot
{
namespace network
{

using asio::ip::tcp;

class FCMServer
{
public:
    void send(asio::io_service & io_service);
}; // class FCMServer
} // namespace network
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP

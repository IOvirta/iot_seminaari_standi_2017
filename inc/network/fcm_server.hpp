#ifndef IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP
#define IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP

#include <iostream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <sstream>
#include <boost/asio/ssl.hpp>

namespace iovirta_iot
{
namespace network
{
	
using boost::asio::ip::tcp;

class FCMServer
{
public:
    void send(boost::asio::io_service & io_service);
}; // class FCMServer
} // namespace network
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP

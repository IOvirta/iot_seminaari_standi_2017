#ifndef IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP
#define IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP

#include <vector>
#include <string>
#include <asio.hpp>

namespace iovirta_iot
{
namespace network
{

using asio::ip::tcp;

class FCMServer
{
private:
    asio::io_service io_service_;
    std::vector<std::string> client_tokens_;
    std::string auth_key_ = "AAAAivefdQY:APA91bGBjV976NHvlV5cSNHp80O-VNYDGF8kJbfBGi_W8iTKh_SsIuRunA615XV_FZ7-LAZgmxcKs1ZZS2GSRmyiPLcQ31PxT0tStPzaG_yvVDJOtza30fuUU9U9rELyB-ZucpIyiLLh";
public:
    FCMServer();
    ~FCMServer()
    {
    }
    void add_client(const std::string &token);
    void send();
}; // class FCMServer
} // namespace network
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_NETWORK_CFM_SERVER_HPP

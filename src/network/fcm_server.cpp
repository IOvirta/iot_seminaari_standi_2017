#include "network/fcm_server.hpp"

using namespace iovirta_iot::network;

void FCMServer::send(boost::asio::io_service & io_service){
	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("fcm.googleapis.com", "443", boost::asio::ip::resolver_query_base::numeric_service);
	//tcp::resolver::query query("127.0.0.1", "http");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	// Try each endpoint until we successfully establish a connection.
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	
	// Form the request. We specify the "Connection: close" header so that the
	// server will close the socket after transmitting the response. This will
	// allow us to treat all data up until the EOF as the content.
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	
	std::stringstream jsontemp;
	jsontemp << "{ \"to\" : \"bk3RNwTe3H0:CI2k_HHwgIpoDKCIZvvDMExUdFQ3P1\" }";
	
	request_stream << "POST /fcm/send HTTP/1.1 \r\n";
	request_stream << "Host:" << "derp" << "\r\n";
	request_stream << "User-Agent: C/1.0";
	request_stream << "Content-Type: application/json\r\n";
	request_stream << "Authorization:key=AAAAmxGRRiA:APA91bFcZAa5uH6Wo-1xX4Y32gKCfj0hit6p0z-9bKhll6Sj_CQq-zRcfekJVB0WKamaGfRZQZusXtwp23v-qEZFlBlnYMgydW2HW9XZpN3aUHgLceDEwfvRL0etLopxoDTnX2D23Squ\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Content-Length: " << jsontemp.str().length() << "\r\n";    
	request_stream << "Connection: close\r\n\r\n";  //NOTE THE Double line feed
	request_stream << jsontemp.str();

	// Send the request.
	boost::asio::write(socket, request);
}

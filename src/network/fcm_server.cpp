#include "network/fcm_server.hpp"

using namespace iovirta_iot::network;

using asio::ip::tcp;
namespace ssl = asio::ssl;
typedef ssl::stream<tcp::socket> ssl_socket;

FCMServer::FCMServer(){
	io_service = new asio::io_service();
}

void FCMServer::send(){

	// Create a context that uses the default paths for
	// finding CA certificates.
	ssl::context ctx(ssl::context::sslv23);
	ctx.set_default_verify_paths();


	//Avataan ssl socket yhteys
	ssl_socket sock((*io_service), ctx);
	tcp::resolver resolver((*io_service));
	tcp::resolver::query query("fcm.googleapis.com", "https");

	asio::connect(sock.lowest_layer(), resolver.resolve(query));
	sock.lowest_layer().set_option(tcp::no_delay(true));

	// Perform SSL handshake and verify the remote host's
	// certificate.
	sock.set_verify_mode(ssl::verify_peer);
	sock.set_verify_callback(ssl::rfc2818_verification("fcm.googleapis.com"));
	sock.handshake(ssl_socket::client);

	/*###OLD METHOD ###
	//tcp::resolver::query query("127.0.0.1", "http");
	 * tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	// Try each endpoint until we successfully establish a connection.
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	* #################*/

	// Form the request. We specify the "Connection: close" header so that the
	// server will close the socket after transmitting the response. This will
	// allow us to treat all data up until the EOF as the content.
	asio::streambuf request;
	std::ostream request_stream(&request);

	std::stringstream jsontemp;
	jsontemp << '"'{
    "to" : "bk3RNwTe3H0:CI2k_HHwgIpoDKCIZvvDMExUdFQ3P1...",
    "notification" : {
      "body" : "Message passed through. Good job.",
      "title" : "Hello commander o7",
      "icon" : "myicon"
    }
  }'"';

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
	asio::write(sock, request);
}

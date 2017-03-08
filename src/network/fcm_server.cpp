#include <iostream>
#include <ostream>
#include <sstream>
#include <asio/ssl.hpp>
#include <string>

#include <asio.hpp>

#include "network/fcm_server.hpp"

using namespace iovirta_iot::network;

using asio::ip::tcp;
namespace ssl = asio::ssl;
typedef ssl::stream<tcp::socket> ssl_socket;

FCMServer::FCMServer(){
	
}

void FCMServer::add_client(const std::string &token)
{
	client_tokens_.push_back(token);
}

void FCMServer::send(){

	// Create a context that uses the default paths for
	// finding CA certificates.
	ssl::context ctx(ssl::context::sslv23);
	ctx.set_default_verify_paths();


	//Avataan ssl socket yhteys
	ssl_socket sock(io_service_, ctx);
	tcp::resolver resolver(io_service_);
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
	std::stringstream request_string;
	std::ostream request_stream(&request);

	std::stringstream jsontemp;
	jsontemp << "{\"data\":{\"viesti\": \"l33t\"},\"to\":\"dWgYhpYI248:APA91bEBJ5Kkq_015lZ_UX1V-0S0VbhDFyMTmZSs0he5_TKM252LA80Fek8yCy5FMkrvAcxB4dDzVLUWBWmCJqTJRhk8gOZWJjL3sEEjP0XJZnNRiqtMNSRYP13ndaUljRgOhG1o-ym4\"}";

	request_string << "POST /fcm/send HTTP/1.1\r\n";
	request_string << "Host:fcm.googleapis.com:443\r\n";
	request_string << "User-Agent: C/1.0\r\n";
	request_string << "Content-Type: application/json\r\n";
	request_string << "Authorization:key=AAAAivefdQY:APA91bGBjV976NHvlV5cSNHp80O-VNYDGF8kJbfBGi_W8iTKh_SsIuRunA615XV_FZ7-LAZgmxcKs1ZZS2GSRmyiPLcQ31PxT0tStPzaG_yvVDJOtza30fuUU9U9rELyB-ZucpIyiLLh\r\n";
	request_string << "Sender: key= 596859909382\r\n";
	request_string << "Accept: */*\r\n";
	request_string << "Content-Length: " << jsontemp.str().length() << "\r\n\r\n";
	request_string << jsontemp.str();

	request_stream << request_string.str();

	// Send the request.
	asio::write(sock, request);

	std::cout << request_string.str() << "\n";

	std::cout << "#####request sent#####\n";
        // Read the response status line.
        asio::streambuf response;
        asio::read_until(sock, response, "\r\n");

        // Check that response is OK.
        std::istream response_stream(&response);

        std::string http_version;
        response_stream >> http_version;

        unsigned int status_code;
        response_stream >> status_code;

	std::cout << http_version;
	std::cout << "\n";
	std::cout << status_code;
	std::cout << "\n";

  // Read the response headers, which are terminated by a blank line.
  asio::read_until(sock, response, "\r\n\r\n");

  // Write whatever content we already have to output.
  if(response.size() > 0){
      std::ostringstream oss;
      oss << &response;
      std::string res = oss.str();
			std::cout << res;
  }

  // Read until EOF, writing data to output as we go.
  //while(asio::read(sock, response, asio::transfer_at_least(1))){
      //std::cout << &response; // don't want to print just return
  //}




}

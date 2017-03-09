#include <iostream>
#include <ostream>
#include <string>

#include <asio/ssl.hpp>
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

	static std::string http_header = "POST /fcm/send HTTP/1.1\r\n\
Host: fcm.googleapis.com:443\r\n\
User-Agent: C/1.0\r\n\
Content-Type: application/json\r\n\
Authorization: key=" + auth_key_ + "\r\n\
Sender: key=596859909382\r\n\
Accept: */*\r\n\
Content-Length: ";

	/* "dry_run" = true -> viestejä ei lähetetä puhelimille */
	std::string fcm_request = R"(
{
	"data" : { "viesti" : "l33t" },
	"priority" : "high",
	"dry_run" : true,
	"registration_ids" : [)";

	/* lisätään kaikkien puhelinten tokenit pyyntöön */
	for (auto it = client_tokens_.begin(); it != client_tokens_.end(); ++it)
	{
		fcm_request += "\"" + *it + "\"";
		if (std::next(it) != client_tokens_.end())
			fcm_request += ",";
		else
			fcm_request += "]}";
	}

	std::string http_request = http_header + std::to_string(fcm_request.length()) + "\r\n\r\n" + fcm_request;

	asio::write(sock, asio::buffer(http_request));

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
}

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
	std::time_t now = time(0);
	return ctime(&now);
}

int main(){
	try
	{
		std::cout << "Initializing daytime server.\n";
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
		
		for (;;){
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			
			std::string message = make_daytime_string();
			
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
			std::cout << "Answered to time request.\n";
		}
	}catch (std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

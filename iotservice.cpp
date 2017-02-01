#include "tcpnetworking.cpp"
#include <boost/asio.hpp>

  
int main()
{
	std::cout << "Starting server.\n Listening for connections.\n";
	
	try
	{
		boost::asio::io_service io_service;
		tcp_server server(io_service);
		io_service.run();
		std::cout << "Script completed\n";
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cout << "error\n";
	}
	
	std::cout << "Server stopped.\n";
	
	return 0;
}


#include <iostream>
#include <boost/asio.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

void print (const boost::system::error_code& ){
	std::cout << "Foo! \n";
} 

int main()
{
	boost::asio::io_service io;
	
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	
	t.async_wait(&print);
	
	std::cout << "testi1\n";
	
	io.run();
	
	std::cout << "testi2\n";
	
	return 0;
	
}

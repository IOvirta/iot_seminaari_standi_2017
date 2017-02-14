#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2) //?????
		{
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}
		
		//io_service objekti tarvitaan aina asio:ta käyttäessä
		boost::asio::io_service io_service;
		
		//Muutetaan io_service tcp resolveriksi
		tcp::resolver resolver(io_service);
		//Query object -> list of endpoints ???
		tcp::resolver::query query(argv[1], "7771");		
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		
		//Luodaan ja yhdistetään socket
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		
		for (;;)
		{
			
			//Luodaan puskuri sisältämään viestit
			boost::array<char, 128> buf;
			boost::system::error_code error;
			
			//Boost.asio.buffer estää buffer overflown
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			
			if (error == boost::asio::error::eof){
				std::cout << "End of communications with the socket. \n";
				break; //Client on sulkenut yhteyden "siististi"
			}
			else if (error){
				throw boost::system::system_error(error); //Jotain muuta
			}
				
			std::cout << "####Start of message:####\n";
			std::cout.write(buf.data(), len);
			std::cout << "####End of message#######\n";
			
			//t::asio::write(*sock, boost::asio::buffer(fullData, sizeof(fullData)));
			std::string data = "Ordersfromclient:1100115";
			boost::asio::write(socket, boost::asio::buffer(data, sizeof(data)) );
			
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

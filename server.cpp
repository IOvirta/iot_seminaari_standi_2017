#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

//Viestin lukua varten
#include <boost/array.hpp>
using boost::asio::ip::tcp;

std::string make_daytime_string()
{
	std::time_t now = std::time(0);
	return ctime(&now);
}

class tcp_connection :
public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	
	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer( new tcp_connection(io_service));
	}
	
	tcp::socket& socket()
	{
		return socket_;
	}
	
	void start()
	{
		std::cout << "Incoming connection.\n";
		message_ = "Current server configuration:11001001000100\n";
		boost::asio::async_write(socket_, boost::asio::buffer(message_),
			boost::bind(&tcp_connection::handle_write, shared_from_this(), 
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
				
		std::cout << "Sent data to client.\n";
				
		//#####Lukutesti#####
		//Luodaan puskuri sisältämään viestit
		boost::array<char, 128> buf;
		boost::system::error_code error;
		
		//Boost.asio.buffer estää buffer overflown
		size_t len = socket_.read_some(boost::asio::buffer(buf), error);
		
		if (error == boost::asio::error::eof){
			std::cout << "End of communications with the socket. \n";
		}
		else if (error){
			throw boost::system::system_error(error); //Jotain muuta
		}
		std::cout.write(buf.data(), len);
		std::cout << "\n";
		//#####Lukutesti#####
				
		
	}
private:
	tcp_connection(boost::asio::io_service& io_service) :
		socket_(io_service)
	{
	}
	
	void handle_write(const boost::system::error_code& /*error*/,
		size_t /*bytes_transferred*/)
	{
	}
	
	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	tcp_server(boost::asio::io_service& io_service) :
		acceptor_(io_service, tcp::endpoint(tcp::v4(), 7771))
	{
		start_accept();
	}
	
private:
	void start_accept()
	{
		tcp_connection::pointer new_connection =
			tcp_connection::create(acceptor_.get_io_service());
			
		acceptor_.async_accept(new_connection->socket(),
			boost::bind(&tcp_server::handle_accept, this, new_connection,
				boost::asio::placeholders::error));
	}
	
	void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
	{
		if(!error){
			new_connection -> start();
		}
		
		start_accept();
	}
	
	tcp::acceptor acceptor_;
};

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

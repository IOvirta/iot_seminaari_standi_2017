#include "tcpnetworking.h"

using boost::asio::ip::tcp;


tcp_connection::pointer tcp_connection::create(boost::asio::io_service& io_service)
{
	return pointer( new tcp_connection(io_service));
}

tcp::socket& tcp_connection::socket()
{
	return socket_;
}

void tcp_connection::start()
{
	std::cout << "Incoming connection.\n";
	std::cout << "Sending server configuration info to client:\n";
	message_ = "TemplateConfigurationInfo777";
	boost::asio::async_write(socket_, boost::asio::buffer(message_),
		boost::bind(&tcp_connection::handle_write, shared_from_this(), 
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
			
	std::cout << "..data sent.\n";
			
	//#####Lukutesti#####
	//Luodaan puskuri sisältämään vastaanotettava viesti
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

tcp_connection::tcp_connection(boost::asio::io_service& io_service) :
	socket_(io_service)
{
}
	
void tcp_connection::handle_write(const boost::system::error_code& /*error*/,
	size_t /*bytes_transferred*/)
{
}

tcp_server::tcp_server(boost::asio::io_service& io_service) :
		acceptor_(io_service, tcp::endpoint(tcp::v4(), 7771))
{
	start_accept();
}
	
void tcp_server::start_accept()
{
	tcp_connection::pointer new_connection =
		tcp_connection::create(acceptor_.get_io_service());
		
	acceptor_.async_accept(new_connection->socket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
{
	if(!error){
		new_connection -> start();
	}
	
	start_accept();
}

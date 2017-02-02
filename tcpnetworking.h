#ifndef _tcpnetworking
#define _tcpnetworking

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


class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	
	static pointer create(boost::asio::io_service& io_service);
	
	tcp::socket& socket();
	
	//Nykyisellään koko tiedonsiirtoko tapahtuu täällä
	void start();
private:

	tcp_connection(boost::asio::io_service& io_service);
	
	void handle_write(const boost::system::error_code&, size_t);
	
	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	//Constructor alustaa acceptorin kuuntelemaan porttia 13,
	//sekä kutsuu start_accept metodia.
	tcp_server(boost::asio::io_service& io_service);
	
private:

	//Luo socketin ja aloittaa asynkronisen accept operaation.
	void start_accept();
	
	//Kutsutaan kun acceptor vastaanottaa yhteyden clientiltä
	//Luo uuden tcp_connection yhteyden
	void handle_accept(tcp_connection::pointer new_connection,
		const boost::system::error_code& error);
	
	//Asion oma luokka..
	tcp::acceptor acceptor_;
};


#endif

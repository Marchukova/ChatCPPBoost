
#ifndef _SERVER_H_
#define _SERVER_H_
#define _WIN32_WINNT 0x0501

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#define BOOST_ASIO_SEPARATE_COMPILATION

class Server {
	boost::asio::io_service service; 
	boost::asio::ip::tcp::endpoint ep;
	boost::asio::ip::tcp::acceptor acc; //object for client's connection

	void readWrite(boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
public:
	Server(const std::string &address, unsigned short port);
	/* Read data from client and write to standart output */
	void run();
};

#endif /* _SERVER_H_ */


#ifndef _SERVER_H_
#define _SERVER_H_
#define _WIN32_WINNT 0x0501

#include <list>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#define BOOST_ASIO_SEPARATE_COMPILATION

class Server {
	boost::asio::io_service service; 
	boost::asio::ip::tcp::endpoint ep;
	boost::asio::ip::tcp::acceptor acc; //object for client's connection
	
	/* List of all current server's clients */
	std::list<boost::shared_ptr<boost::asio::ip::tcp::socket> > clients;
	/* Mutex to work with list of clients */
	boost::mutex clientsMutex;
	
	/* Read data from client and send it to others client */
	void readWrite(boost::shared_ptr<boost::asio::ip::tcp::socket> & sockPtr);
public:
	Server(const std::string &address, unsigned short port);
	
	/* Connect new client and start a thread for it */
	void run();
};

#endif /* _SERVER_H_ */

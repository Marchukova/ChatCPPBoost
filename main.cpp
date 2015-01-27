#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/shared_ptr.hpp>
#include "server.h"

using namespace boost::asio;

/* Read data from client and write to standart output 
/* Thread function
*/
void readWrite(boost::shared_ptr<ip::tcp::socket> sock) {
	try {
		/* read data from client */ 
		char c[1];
		while (sock->read_some(buffer(c))) {
			std::cout << c[0];
		}
	} catch (boost::system::system_error) { //connection was closed
	}
} /* End of 'readWrite' function */

int main() {
	io_service service; 
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 1234);
	ip::tcp::acceptor acc(service, ep); //object for client's connection

	while (true) {
		boost::shared_ptr<ip::tcp::socket> sock_ptr(new ip::tcp::socket(service));
		acc.accept(*sock_ptr); //wait connection
		boost::thread thread(readWrite, sock_ptr); //Start a thread for new client
	}

	return 0;
} /* End of 'main' function */

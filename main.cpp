#include <iostream>
#include "server.h"
using namespace boost::asio;

int main() {
	io_service service; 
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 1234);
	ip::tcp::acceptor acc(service, ep); //object for client's connection
	ip::tcp::socket sock(service);
	acc.accept(sock); //wait connection

	/* read data from client */ 
	try {
		char c[1];
		while (sock.read_some(buffer(c))) {
			std::cout << c[0];
		}
	} catch (boost::system::system_error & e) { //connection was closed
	}

	return 0;
} /* End of 'main' function */


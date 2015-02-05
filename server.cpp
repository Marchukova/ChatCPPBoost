#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "server.h"

using namespace boost::asio;
using namespace std;

/* Constructor */
Server::Server(const std::string &address, unsigned short port) 
	: ep(ip::address::from_string(address), port), acc(service, ep) {
}

/* Read data from client and write to standart output. 
/* Thread function.
*/
void Server::readWrite(boost::shared_ptr<ip::tcp::socket> sock) {
	try {
		boost::asio::streambuf buf;
		//ostream out(&buf); //stream to write to the buffer
		while (read_until(*sock, buf, "\n")) { 	// read data from client 
			cout << &buf; // dump everything in console
		}
	} catch (boost::system::system_error) { //connection was closed
	}
} /* End of 'readWrite' function */

void Server::run() {
	while (true) {
		boost::shared_ptr<ip::tcp::socket> sock_ptr(new ip::tcp::socket(service));
		acc.accept(*sock_ptr); //wait connection
		boost::thread thread(&Server::readWrite, this, sock_ptr); //Start a thread for new client
	}
}


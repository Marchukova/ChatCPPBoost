#include <iostream>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>

#include "server.h"

using namespace boost::asio;
using namespace std;

/* Constructor */
Server::Server(const std::string &address, unsigned short port) 
	: ep(ip::address::from_string(address), port), acc(service, ep) {
}

/* If a pointer is unique, then thread for the pointer is cancelled.
/* The function for deletion.
*/
bool lastPtr(boost::shared_ptr<ip::tcp::socket> & element, ip::tcp::socket *socket) {
	if (element.get() == socket)
		return false;
	return element.unique();
} /* End of 'lastPtr' function */

/* Read data from client and send it to others client. 
/* Thread function.
*/
void Server::readWrite(boost::shared_ptr<ip::tcp::socket> & sockPtr) {
	try {
		boost::asio::streambuf buf;
		while (read_until(*sockPtr, buf, "\n")) { 	// read data from client 
			clientsMutex.lock();
				/* Delete all client who disconnect(but not current) */
				clients.remove_if(boost::bind(lastPtr, _1, sockPtr.get()));
				/* Send message to others */
				for (list<boost::shared_ptr<ip::tcp::socket> >::iterator it = clients.begin();
						it != clients.end(); ++it) 
					if (it->get() != sockPtr.get()) { //send if not current socket
						write(**it, buf.data());
					}		
			clientsMutex.unlock();			
			cout << &buf; // dump everything in console
		}
	} catch (boost::system::system_error) { //connection was closed
	}
} /* End of 'Server::readWrite' function */

/* Connect new client and start a thread for it. 
*/
void Server::run() {
	while (true) {
		boost::shared_ptr<ip::tcp::socket> sockPtr(new ip::tcp::socket(service));
		acc.accept(*sockPtr); //wait for connection
		
		clientsMutex.lock();
			clients.push_back(sockPtr);
			boost::thread thread(&Server::readWrite, this, sockPtr); //Start a thread for a new client
		clientsMutex.unlock();
	}
} /* End of 'Server::run' function */


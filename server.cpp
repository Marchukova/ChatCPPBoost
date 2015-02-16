#include <iostream>
#include <vector>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>

#include "server.h"

using namespace boost::asio;
using namespace std;

/* Constructor */
Server::Server(const std::string &address, unsigned short port, int numLastMessages) 
	: ep(ip::address::from_string(address), port), acc(service, ep), lastMessages(numLastMessages) {
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
		/* First, send last messages to client */
		lastMessages.lock();
			for (FixSizeLockQueue<string>::const_iterator 
					it = lastMessages.cbegin();
					it != lastMessages.cend(); ++it) {
				write(*sockPtr, boost::asio::buffer(*it));
			}
		lastMessages.unlock();

		boost::asio::streambuf buf;
		size_t header;
		vector<boost::asio::const_buffer> buffers(2);
		while (read(*sockPtr, boost::asio::buffer(&header, sizeof(header)))) { 	// read data from client 
			read(*sockPtr, buf.prepare(header));
			buf.commit(header);
			buffers[0] = boost::asio::buffer(&header, sizeof(header));
			buffers[1] = boost::asio::buffer(buf.data(), header);
			clientsMutex.lock();
				/* Delete all client who disconnect(but not current) */
				clients.remove_if(boost::bind(lastPtr, _1, sockPtr.get()));
				/* Send message to others */
				for (list<boost::shared_ptr<ip::tcp::socket> >::iterator it = clients.begin();
						it != clients.end(); ++it) 
					if (it->get() != sockPtr.get()) { //send if not current socket
						write(**it, buffers);
					}	
			clientsMutex.unlock();			
			/* Save in last messages */
			string s((const char *)&header, sizeof(header));
			s += string(boost::asio::buffer_cast<const char*>(buf.data()), header);
			lastMessages.put(s);
			buf.consume(header);
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


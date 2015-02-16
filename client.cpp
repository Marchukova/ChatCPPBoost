
#define _WIN32_WINNT 0x0501

#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "message.h"

using namespace boost::asio;

class Client {
	io_service service; 
	ip::tcp::socket socket;
	boost::thread toServer, fromServer;
	std::string userName;

	void sendToServer() {
		try {
			boost::asio::streambuf buf;
			std::ostream os(&buf);
			std::vector<boost::asio::const_buffer> buffers(2);
			while (true) {
				std::string s;
				std::getline(std::cin, s); 
				boost::archive::binary_oarchive archive(os);
				Message msg(userName, s + "\n");				
				archive << msg; //serialize message
				const size_t header = buf.size(); //size of serialized message
				/* Size of serialized object */				
				buffers[0] = boost::asio::buffer(&header, sizeof(header));
				/* Serialized object */
				buffers[1] = boost::asio::buffer(buf.data(), header);
				boost::asio::write(socket, buffers);
				buf.consume(header);  //clear input sequence
			}
		} catch (boost::system::system_error) { //connection was closed
		}
	}

	void getFromServer() {
		try {
			boost::asio::streambuf buf;
			std::istream in(&buf);
			/* Read header */
			size_t header;
			while (read(socket, boost::asio::buffer(&header, sizeof(header)))) { 	// read data from server
				/* Read body */
				read(socket, buf.prepare(header));
			    buf.commit(header);  //prepare streambuf for input
				/* Deserialization */
				boost::archive::binary_iarchive archive(in);
				Message msg;
				archive >> msg;
				std::cout << msg.getMessage(); 
			}
		} catch (boost::system::system_error) { //connection was closed
		}
	}

public:
	Client(const std::string & address, unsigned short port, const std::string & userName) 
		: socket(service), userName(userName) {
		/* Connects to the server */
		ip::tcp::endpoint ep(ip::address::from_string(address), port);
		socket.connect(ep);
		toServer = boost::thread(&Client::sendToServer, this);
		fromServer = boost::thread(&Client::getFromServer, this);
	}

	void join() {
		toServer.join();
		fromServer.join();
	}
};

int main() {
	std::cout << "Please, enter your name" << std::endl;
	std::string s;
	std::getline(std::cin, s);
	Client client("127.0.0.1", 1234, s);
	client.join();
	return 0;
} /* End of 'main' function */

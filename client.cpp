
#define _WIN32_WINNT 0x0501

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;

class Client {
	io_service service; 
	ip::tcp::socket socket;
	boost::thread toServer, fromServer;

	void sendToServer() {
		try {
			while (true) {
				std::string s;
				std::cin >> s;
				socket.write_some(buffer(s + "\n"));
			}
		} catch (boost::system::system_error) { //connection was closed
		}
	}

	void getFromServer() {
		try {
			boost::asio::streambuf buf;
			//ostream out(&buf); //stream to write to the buffer
			while (read_until(socket, buf, "\n")) { 	// read data from client 
				std::cout << &buf; // dump everything in console
			}
		} catch (boost::system::system_error) { //connection was closed
		}
	}

public:
	Client(const std::string &address, unsigned short port) : socket(service) {
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
	Client client("127.0.0.1", 1234);
	client.join();
	return 0;
} /* End of 'main' function */


#define _WIN32_WINNT 0x0501

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

class Client {
	io_service service; 
	ip::tcp::socket socket;
public:
	Client(const std::string &address, unsigned short port) : socket(service) {
		ip::tcp::endpoint ep(ip::address::from_string(address), port);
		socket.connect(ep);
	}

	void run() {		
		std::string s;
		std::cin >> s;
		socket.write_some(buffer(s));
	}
};

int main() {
	Client c("127.0.0.1", 1234);
	c.run();
	return 0;
} /* End of 'main' function */

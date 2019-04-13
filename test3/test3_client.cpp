// g++ -std=c++11 test3_client.cpp -o test3_client.out -lpthread

#include <iostream>
#include <vector>
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;

int main() {
	try {
		std::cout << "client start. ";

		io_service io;

		ip::tcp::socket sock(io);
		ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 50000);
		sock.connect(ep);
		std::cout << sock.available() << std::endl;

		std::vector<char> str(sock.available() + 1, 0);
		sock.receive(buffer(str));

		std::cout << "receive from " << sock.remote_endpoint().address() << std::endl;
		std::cout << &str[0] << std::endl;
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}


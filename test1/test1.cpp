// g++ test1.cpp -o test1.out -lpthread
// signal handler example with asio

// kill -10 test1.out
// liu@liu-VirtualBox:~/boost/test1$ ./test1.out 
// @asio|1554635512.117640|0*1|signal_set@0xbfa4a4a4.async_wait
// @asio|1554635512.117741|0*2|signal_set@0xbfa4a4a4.async_wait
// @asio|1554635561.578633|>1|ec=system:0,signal_number=10
// @asio|1554635561.578644|<1|
// @asio|1554635561.578655|>2|ec=system:0,signal_number=10
// sig_usr_handler recv = 10
// @asio|1554635561.578699|<2|
// io stopped and exit
// @asio|1554635561.578733|0|signal_set@0xbfa4a4a4.cancel
// liu@liu-VirtualBox:~/boost/test1$ 

#define BOOST_ASIO_DISABLE_STD_CHRONO
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <csignal>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::system;

void sig_int_handler (const error_code& ec, int signal_number) {
	if (ec) {
		std::cout << ec.message() << std::endl;
	}

	if (signal_number == SIGINT) {
		std::cout << "sig_int_handler recv = " << signal_number << std::endl;
	}
	return;
}

void sig_usr_handler(const error_code& ec, int signal_number) {
        if (ec) {
                std::cout << ec.message() << std::endl;
        }

        if (signal_number == SIGUSR1) {
                std::cout << "sig_usr_handler recv = " << signal_number << std::
endl;
        }

	// attach the handler to signal again for repeat
	// sig.async_wait(sig_int_handler);
        // sig.async_wait(sig_usr_handler);

	return;
}

int main() {
	io_service io;

	// bind signal with io context
	signal_set sig(io, SIGINT, SIGUSR1);

	// attach signal handler
	sig.async_wait(sig_int_handler);
	sig.async_wait(sig_usr_handler);

	io.run();

	std::cout << "io stopped and exit" << std::endl;

	return 0;
}

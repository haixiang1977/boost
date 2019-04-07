// g++ -std=c++11 test2.cpp -o test2.out -lpthread
// timer handler example with asio
// liu@liu-VirtualBox:~/boost/test2$ ./test2.out 
// @asio|1554645136.166124|0*1|deadline_timer@0xbfd22dfc.async_wait
// @asio|1554645141.167291|>1|ec=system:0
// hello asio timer
// @asio|1554645141.167396|1|deadline_timer@0xbfd22dfc.cancel
// @asio|1554645141.167409|1*2|deadline_timer@0xbfd22dfc.async_wait
// @asio|1554645141.167425|<1|
// @asio|1554645145.167464|>2|ec=system:0
// hello asio timer
// @asio|1554645145.167496|2|deadline_timer@0xbfd22dfc.cancel
// @asio|1554645145.167504|2*3|deadline_timer@0xbfd22dfc.async_wait
// @asio|1554645145.167514|<2|
// @asio|1554645149.167549|>3|ec=system:0
// hello asio timer

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/function.hpp>
#include <chrono>

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

int main() {
	io_service io;
	steady_timer t(io); // timer stop in 5 seconds
	t.expires_from_now(std::chrono::milliseconds(5000));

	function<void(const error_code&)> handler = // lambada 
		[&] (const error_code& ec) {
			if (ec) {
				std::cout << ec.message() << std::endl;
			}
			std::cout << "hello asio timer" << std::endl;

			// register the handler again to the timer
			t.expires_from_now(std::chrono::milliseconds(4000));
			t.async_wait(handler);
		};

	t.async_wait(handler); // attach handler to timer

	io.run();

	return 0;
}


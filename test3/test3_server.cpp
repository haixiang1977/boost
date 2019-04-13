// g++ -std=c++11 test3_server.cpp -o test3_server.out -lpthread
// network block handler example with asio
// https://www.zhihu.com/question/42159518
// https://segmentfault.com/a/1190000007225464
// 为什么很多人说它性能不好，原因其实很简单。
// Asio 给出的标准实例，是单个contex可以多线程run，使用该contex进行分发回调。
// 这个模型在window 上的iocp 实现，简直完美，因为接口都是系统api，各个线程等待完成事件都是不需要锁来等待的。锁只需要保护队列即可。
// Linux 平台，使用epoll模拟，导致一个contex在多个线程run会有一把大锁直接锁调用。其实多线程run就是不同线程切换run，性能的确不行。
// 但是，linux 平台，你可以改变使用习惯，用一个contex进行accept拿到连接，再建立一个contex池，把连接的处理抛到池中执行。跟默认epoll模型一致，也就可以了。
// https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/examples/cpp11_examples.html

#include <iostream>
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;

int main() {
	try {
		std::cout << "server started: ";

		io_service io;

		ip::tcp::acceptor acceptor(io,
			ip::tcp::endpoint(ip::tcp::v4(), 50000));
		std::cout << acceptor.local_endpoint().address() << std::endl;

		while (1) {
			ip::tcp::socket sock(io);
			acceptor.accept(sock);

			std::cout << "client: ";
			std::cout << sock.remote_endpoint().address() << std::endl;

			sock.send(buffer("server: hello asio"));
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}


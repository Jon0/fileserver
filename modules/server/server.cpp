#include <thread>

#include "socket.h"
#include "server.h"


void init(core::engine &e) {
	core::channel *c = new os::tcp_acceptor(e, 8080);
	e.channel_open(c);
}


namespace os {


void sockaccept(core::engine &e) {

}

void sockctl(core::channel &c) {
	std::cout << "sockctl called\n";
	sockaccept(c.get_engine());
	//std::thread t(sockaccept, c.get_engine());
	//t.join();
}

void sendtcp(core::channel &c) {
	std::cout << "sendtcp called\n";
}


}

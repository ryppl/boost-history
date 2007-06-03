// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

/*#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define _WIN32_WINDOWS
#endif*/
#include <boost/signal_network/socket_receiver.hpp>
#include <boost/signal_network/socket_sender.hpp>
#include <boost/signal_network/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;


// asio test
boost::mutex mutex_;
boost::condition cond;
asio::io_service io_service;

// This function will set up an asio acceptor, and wait for a connection.
// Once established, it will set up a signal network which will send
// its final signal through the socket.
void asio_server()
{
	// set up the socket
	asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1097));
	asio::ip::tcp::socket socket(io_service);
	{
		boost::mutex::scoped_lock lock(mutex_);
		acceptor.listen();
		cond.notify_all();
	}
	acceptor.accept(socket);

	// instantiate the components - a float generator, a filter that adds 2, and a sender
	signet::storage<void (float)>::unfused generator(1.0f);
    signet::function<float(float)>::unfused add2(boost::bind(std::plus<float>(), _1, 2.0f));
    signet::socket_sender<void (float)>::unfused sender(socket);

	// create the network
	generator >>= add2 >>= sender;

	// cause the generator to send it's stored value
	generator();
}

int test_main(int, char* [])
{
	// start the server in a separate thread
	boost::mutex::scoped_lock lock(mutex_);
	boost::thread t(asio_server);
	cond.wait(lock);

	// set up the socket
	asio::ip::tcp::endpoint endpoint_recv(asio::ip::address::from_string("127.0.0.1"), 1097);
	asio::ip::tcp::socket socket(io_service);
	socket.connect(endpoint_recv);

	// instatiate the components
    signet::socket_receiver<void (float)> receiver(socket);
	signet::storage<void (float)>::unfused collector(0.0f);

	// set up the network
	receiver >>= collector;

	// this receiver is synchronous - we have to tell it to receive a signal
	receiver();

	BOOST_CHECK(collector.value_<0>() == 3.0f);

	t.join();

    return 0;
} // int test_main(int, char* [])

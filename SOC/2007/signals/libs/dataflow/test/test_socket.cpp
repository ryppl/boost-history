// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio.hpp>

#include <boost/dataflow/signal/component/socket_receiver.hpp>
#include <boost/dataflow/signal/component/socket_sender.hpp>
#include <boost/dataflow/signal/component/function.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

//[ test_socket

using namespace boost;

mutex mutex_;
condition cond;
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
	signals::storage<void (float), signals::unfused> generator(1.0f);
    signals::function<void (float), float(float), signals::unfused> add2(boost::bind(std::plus<float>(), _1, 2.0f));
    signals::socket_sender<void (float)> sender(socket);

	// create the network
	generator >>= add2 >>= sender;

	// cause the generator to send it's stored value
	generator.send();
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
    signals::socket_receiver<void (float), signals::fused> receiver(socket);
	signals::storage<void (float), signals::fused> collector(0.0f);

	// set up the network
	receiver >>= collector;

	// this receiver is synchronous - we have to tell it to receive a signal
	receiver();

	BOOST_CHECK_EQUAL(collector.at<0>(), 3.0f);

	t.join();

    return 0;
} // int test_main(int, char* [])
//]

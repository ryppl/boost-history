// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ simple_distributed_example

#include <boost/asio.hpp>

#include <boost/dataflow/signal/component/socket_receiver.hpp>
#include <boost/dataflow/signal/component/socket_sender.hpp>
#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/component/timed_generator.hpp>
#include <boost/dataflow/signal/connection.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <iostream>

using namespace boost;

// This will be our data processor.  The signature void(double) designates
// the output signal (we will be sending out an double).  The signals
// we can receive depend on how we overload operator().
class processor : public signals::filter<void (double)>
{
public:
    // Initialize the Gaussian noise generator.
    processor() : generator(mt, dist) {}
    
    // Receive void(double) signals, add some Gaussian noise, and send
    // out the modified value.
    void operator()(double x)
    {
        out(x + generator());
    }
private:
    mt19937 mt;
    normal_distribution<> dist;
    boost::variate_generator<mt19937&, boost::normal_distribution<> > generator;
};

// This will be our data output.  We just need to make a function object,
// and specify that it is a signals::call_consumer.
class output
{
public:
    typedef dataflow::signals::call_consumer port_traits;
    
    void operator()(double x)
    {
        std::cout << x << std::endl;
    }
};

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

    // For our data source, we will use timed_generator,
    // which creates its own thread and outputs it's stored value
    // at a specified time interval.  We'll store a value of 0 to be sent out.
    // The signature void(double) specifies that the signal carries a double,
    // and that there is no return value.
    signals::timed_generator<void (double), signals::unfused> input(0);
	// To have our dataflow network straddle a network connection,
    // we need a socket_sender
    signals::socket_sender<void (double)> sender(socket);

	// create the network
	input >>= sender;

    // Tell the source to start producing data, every 0.5s:
    input.enable(0.5);
    
    // take a little nap :-)
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += 10;
    boost::thread::sleep(xt);

    input.join();
}

int main(int, char* [])
{
	// start the server in a separate thread, and wait until it is listening
	boost::mutex::scoped_lock lock(mutex_);
	boost::thread t(asio_server);
	cond.wait(lock);

	// set up the socket
	asio::ip::tcp::endpoint endpoint_recv(asio::ip::address::from_string("127.0.0.1"), 1097);
	asio::ip::tcp::socket socket(io_service);
	socket.connect(endpoint_recv);

    // Setup data processor and output:
    processor proc;
    output out;
    signals::socket_receiver<void (double), signals::unfused> receiver(socket);

	// set up the network
	receiver >>= proc >>= out;

    // start asynchronous receiving
    receiver.async_read();
    boost::thread receive_thread(boost::bind(&asio::io_service::run, boost::ref(io_service)));
    
    // and wait until the server is done sending
	t.join();
    
    io_service.stop();
    receive_thread.join();

    return 0;
} // int test_main(int, char* [])
//]

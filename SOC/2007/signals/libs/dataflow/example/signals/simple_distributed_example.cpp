// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


//[ simple_distributed_example

#include <boost/dataflow/signals/component/socket_receiver.hpp>
#include <boost/dataflow/signals/component/socket_sender.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/timed_generator.hpp>
#include <boost/dataflow/signals/connection.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "simple_example_components.hpp"


using namespace boost;


mutex mutex_;
condition cond;
bool server_thread_ready = false;
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
        server_thread_ready  = true;
        cond.notify_all();
    }
    acceptor.accept(socket);

    // For our data source, we will use timed_generator,
    // which creates its own thread and outputs it's stored value
    // at a specified time interval.  We'll store a value of 0 to be sent out.
    // The signature void(double) specifies that the signal carries a double,
    // and that there is no return value.
    signals::timed_generator<void (double)> input(0);
    // To have our dataflow network straddle a network connection,
    // we need a socket_sender
    signals::socket_sender<void (double)> sender(socket);

    // ---Connect the dataflow network ---------------------
    //
    //     ,---------.     ,---------.
    //     |  input  | --> |  sender  --- -- - (socket)
    //     `---------'     `---------'
    //
    // -----------------------------------------------------
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
    boost::thread t(asio_server);
    {
        boost::mutex::scoped_lock lock(mutex_);
        while(!server_thread_ready)
            cond.wait(lock);
    }

    // set up the socket
    asio::ip::tcp::endpoint endpoint_recv(asio::ip::address::from_string("127.0.0.1"), 1097);
    asio::ip::tcp::socket socket(io_service);
    socket.connect(endpoint_recv);

    // Setup data processor and output:
    processor proc;
    output out;
    signals::socket_receiver<void (double)> receiver(socket);

    // ---Connect the dataflow network -----------------------------
    //
    //                  ,---------.     ,---------.     ,---------.
    // (socket) - -- --- receiver | --> |  proc   | --> |   out   |
    //                  `---------'     `---------'     `---------'
    //
    // -------------------------------------------------------------
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

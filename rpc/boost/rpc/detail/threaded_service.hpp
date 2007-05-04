#ifndef BOOST_RPC_THREADED_SERVICE_HPP
#define BOOST_RPC_THREADED_SERVICE_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace boost {
namespace rpc {
namespace detail {

/// Functionality of an asio io_service which is left permanently running
/// with an associated thread.
class threaded_service
{
protected:
    /// Associates a permanent work with the service so it doesn't exit its run method
    /// until stopped.
    threaded_service() : t(0), work(service) {}
    /// Starts the thread and runs the service.
    void start_service_thread()
    {
        t = new boost::thread(boost::bind(&threaded_service::service_thread, this));
    }
    /// Stops the service and joins the thread.
    void stop_service_thread()
    {
        service.stop();
        if (t)
        {
            t->join();
            delete t;
        }
    }
private:
    void service_thread()
    {
        try
        {
            service.run();
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
protected:
    boost::asio::io_service service;
private:
    boost::asio::io_service::work work;
    boost::thread *t;
};

} // namespace detail
} // namespace rpc
} // namespace boost

#endif
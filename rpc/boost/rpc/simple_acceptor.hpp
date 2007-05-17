#ifndef BOOST_RPC_SIMPLE_ACCEPTOR
#define BOOST_RPC_SIMPLE_ACCEPTOR

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace boost {
namespace rpc {

/// Accepts a connection upon request.
/** \warning Using this acceptor is unsecure.
*/
class simple_acceptor
{
public:
    simple_acceptor(boost::asio::io_service& io_service, int port_number,
        boost::function<void (std::auto_ptr<boost::asio::ip::tcp::socket>)> callback)
        : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_number))
        , connected(callback)
    {
        start_accept();
    }
    ~simple_acceptor()
    {
        delete socket_ptr;
    }
private:
    void start_accept()
    {
        socket_ptr = new boost::asio::ip::tcp::socket(acceptor_.io_service());
        acceptor_.async_accept(*socket_ptr,
            boost::bind(&simple_acceptor::handle_accept, this,
            boost::asio::placeholders::error));
    }

    void handle_accept(const boost::system::error_code& error)
    {
        if (!error)
        {
            connected(std::auto_ptr<boost::asio::ip::tcp::socket> (socket_ptr));
            start_accept();
        }
    }

    boost::asio::ip::tcp::socket * socket_ptr;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::function<void (std::auto_ptr<boost::asio::ip::tcp::socket>)> connected;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SIMPLE_ACCEPTOR

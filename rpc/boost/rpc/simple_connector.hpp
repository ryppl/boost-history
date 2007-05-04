#ifndef BOOST_RPC_SIMPLE_CONNECTOR
#define BOOST_RPC_SIMPLE_CONNECTOR

#include <boost/asio.hpp>

namespace boost {
namespace rpc {

class simple_connector
{
public:
 simple_connector(const asio::ip::tcp::endpoint &endpoint_) : endpoint_recv(endpoint_) {}
    void connect(boost::asio::ip::tcp::socket &socket)
  {
       socket.connect(endpoint_recv);
  }
private:
   boost::asio::ip::tcp::endpoint endpoint_recv;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SIMPLE_CONNECTOR

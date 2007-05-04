#ifndef BOOST_RPC_SOCKET_STRING_SENDER_HPP
#define BOOST_RPC_SOCKET_STRING_SENDER_HPP

#include <boost/asio.hpp>
#include <string>

namespace boost {
namespace rpc {

class socket_string_sender
{
public:
    socket_string_sender(asio::ip::tcp::socket &socket) : socket_(socket) {}
    send(const std::string &str)
    {
        boost::asio::write(socket_, asio::buffer(str), boost::asio::transfer_all());
    }
private:
    asio::ip::tcp::socket &socket_;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SOCKET_STRING_SENDER_HPP

#ifndef BOOST_RPC_SOCKET_STRING_RECEIVER_HPP
#define BOOST_RPC_SOCKET_STRING_RECEIVER_HPP

#include <boost/asio.hpp>
#include <string>

namespace boost {
namespace rpc {

class socket_string_receiver
{
    socket_string_receiver(asio::ip::tcp::socket &socket) : socket_(socket)
    {
        boost::asio::async_read(socket_, boost::asio::buffer(data),
            boost::bind(&chat_client::handle_read, this,
            boost::asio::placeholders::error));
    }
    void handle_read(const boost::asio::error& error)
    {

    }
private:
    asio::ip::tcp::socket &socket_;
    std::string data;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SOCKET_STRING_RECEIVER_HPP

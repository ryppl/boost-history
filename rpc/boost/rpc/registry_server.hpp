// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_REGISTRY_SERVER
#define BOOST_RPC_REGISTRY_SERVER

#include <boost/rpc/config.hpp>
#include <boost/rpc/detail/expanding_buffer.hpp>
#include <boost/rpc/registry.hpp>
#include <boost/rpc/call_options.hpp>
#include <boost/rpc/detail/protocol.hpp>
#include <boost/shared_ptr.hpp>
#include <sstream>

namespace boost {
namespace rpc {

/// Services calls from a single connection with the server.
/** \param Registry Type of registry used with the server.
*/
template<typename Registry>
class registry_server
{
public:
    typedef typename Registry::id_type id_type;
    typedef typename Registry::archive_type archive_type;

    /// Initializes the registry server with the specified registry and socket.
    registry_server(Registry &reg, boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
        : marshal_buffer(BOOST_RPC_RPC_BUFFER_SIZE),
        stream(std::ios::in | std::ios::out | std::ios::binary),
        registry_(reg), socket_ptr(sock)
    {
       prepare_async_read();
       protocol::write_message(*socket_ptr, protocol::ok_message);
    }
private:
    void prepare_async_read()
    {
        boost::asio::async_read(*socket_ptr, request_header.as_buffer(),
            boost::bind(&registry_server::read_header, this,
            boost::asio::placeholders::error));
    }
    void read_header(const boost::system::error_code& error)
    {
        if (!error)
        {
            boost::asio::async_read(*socket_ptr,
                boost::asio::buffer(marshal_buffer, request_header.marshal_size),
                boost::bind(&registry_server::read_marshal, this, boost::asio::placeholders::error));
            marshal_buffer.guarantee(request_header.marshal_size);
        }
    }
    void read_marshal(const boost::system::error_code& error)
    {
        std::string result;
        protocol::call_footer footer;
        boost::asio::read(*socket_ptr, footer.as_buffer(), boost::asio::transfer_all());
     
        registry_(std::string(marshal_buffer, request_header.marshal_size),
            footer.options, &result);

        if (footer.options.marshal_option >= call_options::completed_only)
        {
            request_header.marshal_size = static_cast<protocol::marshal_size_type>(result.size());
            socket_ptr->send(request_header.as_buffer());
        }
        if (footer.options.marshal_option >= call_options::return_only)
        {
            socket_ptr->send(boost::asio::buffer(result));
        }
        // prepare next read
        prepare_async_read();
    }

    protocol::call_header request_header;
    detail::expanding_buffer marshal_buffer;
    std::stringstream stream;

    Registry &registry_;
    boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_REGISTRY_SERVER

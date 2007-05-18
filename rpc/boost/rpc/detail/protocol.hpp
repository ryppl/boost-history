#ifndef BOOST_RPC_PROTOCOL_HPP
#define BOOST_RPC_PROTOCOL_HPP

#include <boost/cstdint.hpp>
#include <boost/asio.hpp>
#include <boost/rpc/call_options.hpp>
#include <boost/future/future.hpp>

namespace boost {
namespace rpc {
namespace protocol {
namespace message
{
    /// Message types used in the communication.
    enum category
    {
        ok = 0x4F4B,
        call = 0x434C
    };
}

typedef boost::int32_t message_category_type;
typedef boost::int32_t request_id_type;
typedef boost::uint64_t marshal_size_type;

const message_category_type ok_message = message::ok;

/// Header for a call request from the client.
/** Contains the request id, call options, and size of the marshal part of the buffer.
*/
struct call_header
{
    /// Does not initialize the header.
    call_header() {}
    /// Initializes the call header with the specified values.
    call_header(request_id_type request_id, marshal_size_type marshal_size)
        : message_category(message::ok),
        request_id(request_id),
        marshal_size(marshal_size) {}

    /// message_type is call_message.
    message_category_type message_category;
    /// Id associated with this call request
    request_id_type request_id;
    /// Size of the marshal part of the package.
    marshal_size_type marshal_size;

    boost::asio::mutable_buffers_1 as_buffer()
    {
        return boost::asio::buffer(this, sizeof(call_header));
    }
};

struct call_footer
{
    call_footer(){}
    call_footer(const boost::rpc::call_options &options)
        : options(options)
    {}

    /// Options of the call being made.
    boost::rpc::call_options options;

    boost::asio::mutable_buffers_1 as_buffer()
    {
        return boost::asio::buffer(this, sizeof(call_footer));
    }
};

/// Header for a call request from the client
struct client_side_call_header
{
    /// Pointer to the client-side call structure.
    intptr_t call_ptr;
};

/// Waits for a message on the socket and returns it.
/** \todo What if an error happened?
*/
template<typename Socket>
message_category_type read_message(Socket &socket)
{
    message_category_type category;
    boost::asio::read(socket, boost::asio::buffer(&category, sizeof(category)));
    return category;
}

template<typename Socket>
class message_reader
{
public:
    message_reader(Socket &socket) : future(promise)
    {
        boost::asio::async_read(socket, boost::asio::buffer(&category, sizeof(category)),
            boost::bind(&message_reader::read_message, this,
            boost::asio::placeholders::error));
    };
    message_category_type message()
    {
        for (int i=1; i<1000; i++)
            std::cout << "";
        return future;
    }
protected:
    void read_message(const boost::system::error_code& error)
    {
        if (!error)
            promise.set(category);
    }
    boost::promise<message_category_type> promise;
    boost::future<message_category_type> future;
    message_category_type category;
};

/// Sends a message on the socket.
/** \todo What if an error happened?
*/
template<typename Socket>
void write_message(Socket &socket, const message_category_type &category)
{
    boost::asio::write(socket, boost::asio::buffer(&category, sizeof(category)));
}

} // namespace protocol
} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_PROTOCOL_HPP

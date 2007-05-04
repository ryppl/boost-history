#ifndef BOOST_RPC_PROTOCOL_HPP
#define BOOST_RPC_PROTOCOL_HPP

#include <boost/cstdint.hpp>
#include <boost/asio.hpp>

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

    enum sync_options
    {
        sync = 0,
        async = 1
    };
}

typedef boost::int32_t message_category_type;
typedef boost::int32_t request_id_type;
typedef boost::uint64_t marshal_size_type;
typedef boost::int8_t sync_type;

const message_category_type ok_message = message::ok;

/// Header for a call request from the client.
/** Contains the request id, call options, and size of the marshal part of the buffer.
*/
struct call_header
{
    /// Does not initialize the header.
    call_header() {}
    /// Initializes the call header with the specified values.
    call_header(request_id_type request_id, const boost::rpc::call_options &options,
         sync_type sync, marshal_size_type marshal_size)
        : message_category(message::ok),
        request_id(request_id),
        options(options),
        sync(sync),
        marshal_size(marshal_size) {}

    /// message_type is call_message.
    message_category_type message_category;
    /// Id associated with this call request
    request_id_type request_id;
    /// Options of the call being made.
    boost::rpc::call_options options;
    /// True if the call is to be made asynchronously
    sync_type sync;
    /// Size of the marshal part of the package.
    marshal_size_type marshal_size;

    boost::asio::mutable_buffers_1 as_buffer()
    {
        return boost::asio::buffer(this, sizeof(call_header));
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

/// Waits for a message on the socket and returns it.
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

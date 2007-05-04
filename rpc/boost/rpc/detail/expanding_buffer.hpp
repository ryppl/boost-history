#ifndef BOOST_RPC_EXPANDING_BUFFER_HPP
#define BOOST_RPC_EXPANDING_BUFFER_HPP

#include <boost/asio/buffer.hpp>

namespace boost {
namespace rpc {
namespace detail {

class expanding_buffer
{
public:
    expanding_buffer(size_t size)
    {
        buffer = new char [size];
        buffer_size = size;
    }
    ~expanding_buffer()
    {
        delete[] buffer;
    }
    void guarantee(size_t size)
    {
        if (buffer_size < size)
        {
            delete[] buffer;
            buffer = new char [buffer_size = size];
        }
    }
    operator char *() {return buffer;}
    boost::asio::mutable_buffers_1 whole() {return boost::asio::buffer(buffer, buffer_size);}
private:
    char *buffer;
    size_t buffer_size;
};

} // namespace detail
} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_EXPANDING_BUFFER_HPP

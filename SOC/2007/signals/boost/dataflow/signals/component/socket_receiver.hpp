// socket_receiver.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SOCKET_RECEIVER_HPP
#define SIGNAL_NETWORK_SOCKET_RECEIVER_HPP

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/detail/serialize_fusion_vector.hpp>

#include <sstream>
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace boost { namespace signals {

/** \brief Receives serializable signals through a Boost.Asio socket.
\param Signature Signature of the signal sent (and received through the socket).
\todo socket_sender only works for Signatures of return type void.

*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
>
class socket_receiver : public storage<Signature, OutSignal, SignalArgs>
{
    typedef storage<Signature, OutSignal, SignalArgs> base_type;

public:
    /// Initializes the socket_sender to use the provided socket.
    socket_receiver(asio::ip::tcp::socket &socket) :
		socket_(socket),
		stream(std::ios::in | std::ios::out | std::ios::binary) {}
    /// Sets the receiver to wait for a packet and send its contents via the signal.
	void operator()()
	{
        uint32_t packet_size;
        boost::asio::read(socket_, asio::buffer(&packet_size, 4));
        process_buffer(socket_.receive(asio::buffer(buffer, packet_size)));
	}
    void async_read()
    {
        boost::asio::async_read(
            socket_,
            asio::buffer(&packet_size, 4),
            boost::bind(
                &socket_receiver::async_read_header,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
private:
    void async_read_header(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            boost::asio::async_read(
                socket_,
                asio::buffer(buffer, packet_size),
                boost::bind(
                    &socket_receiver::async_read_signal,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            // unless socket has been closed, throw the error
            if (error != boost::asio::error::eof)
                throw boost::system::system_error(error);
        }
    }

    void async_read_signal(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {       
            process_buffer(bytes_transferred);
            // prepare next read
            async_read();
        }
        else
            throw boost::system::system_error(error);
    }
    
    void process_buffer(size_t bytes_transferred)
    {
        stream.str(empty_string);
        stream.write((char *)buffer, bytes_transferred);
        boost::archive::binary_iarchive archive(stream);
        archive & base_type::stored();
        base_type::send();
    }

	asio::ip::tcp::socket &socket_;
	std::stringstream stream;
	std::string empty_string;
	boost::archive::binary_iarchive *archive;
    uint32_t packet_size;
	enum { bufferSize = 20000 };
	unsigned char buffer[bufferSize];
};

} } // namespace boost::signals

#endif

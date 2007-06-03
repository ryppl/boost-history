// socket_sender.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SOCKET_SENDER_HPP
#define SIGNAL_NETWORK_SOCKET_SENDER_HPP

#include <boost/mpl/vector.hpp>
#include <boost/signal_network/filter.hpp>
#include <boost/asio.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include <boost/signal_network/detail/unfused_inherited.hpp>
#include <boost/signal_network/detail/serialize_fusion_vector.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

template<typename Signature>
class socket_sender
{
public:
    typedef boost::fusion::unfused_inherited<socket_sender<Signature>,
        typename mpl::vector<asio::ip::tcp::socket &>::type,
        typename boost::function_types::parameter_types<Signature> > unfused;

    typedef boost::function_types::parameter_types<Signature> ParTypes;

    socket_sender(asio::ip::tcp::socket & socket) :
        socket(& socket),
		stream(std::ios::in | std::ios::out | std::ios::binary)
    { }
    socket_sender() :
        socket(NULL),
		stream(std::ios::in | std::ios::out | std::ios::binary)
    { }
    virtual ~socket_sender()
    {
    }

    template<class Seq>
    struct result
    { 
        typedef void type;
    };
    template <class Seq>
    void operator()(const Seq &vec_par)
    {
        if (socket)
        {
    		boost::archive::binary_oarchive archive (stream);
            archive & vec_par;
    		boost::asio::write(*socket, asio::buffer(stream.str()),
        		boost::asio::transfer_all());
    		stream.str(empty_string);
        }
    }
private:
	asio::ip::tcp::socket *socket;
	std::stringstream stream;
	std::string empty_string;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif

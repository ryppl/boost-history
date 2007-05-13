// socket_sender.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SOCKET_SENDER_HPP
#define SIGNAL_NETWORK_SOCKET_SENDER_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/asio.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>

#define SIGNAL_NETWORK_SOCKET_PACK(z,n,text) archive & arg##n;

#define SIGNAL_NETWORK_TEMPLATE_CLASS socket_sender
#include <boost/signal_network/detail/loader.hpp>

#undef SIGNAL_NETWORK_SOCKET_PACK

#endif

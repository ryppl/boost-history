// socket_receiver.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SOCKET_RECEIVER_HPP
#define SIGNAL_NETWORK_SOCKET_RECEIVER_HPP

#include <boost/signal_network/storage.hpp>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>

#define SIGNAL_NETWORK_SOCKET_UNPACK(z,n,text) archive & storage<Signature>::arg##n##_value;

#define SIGNAL_NETWORK_TEMPLATE_CLASS socket_receiver
#define SIGNAL_NETWORK_TEMPLATE_BASE storage<Signature>
#include <boost/signal_network/detail/loader.hpp>

#undef SIGNAL_NETWORK_SOCKET_UNPACK

#endif
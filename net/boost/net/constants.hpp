//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_CONSTANTS_H
#define BOOST_NET_CONSTANTS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#ifdef	BOOST_NET_HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#include <sys/uio.h>
}

#include <cstddef>

#include <boost/net/types.hpp>

namespace boost {
namespace net
{
const native_handle_type invalid_native_handle( -1);
const std::size_t default_backlog_size( 1024);
const std::size_t default_mcast_ttl( 255);
const std::size_t fqdn_size( 255);
#ifdef BOOST_NET_EPOLL
const std::size_t max_epoll_events( 30000);
#endif
#ifdef BOOST_NET_HAVE_IOVEC_STRUCT
const std::size_t max_iovecs( IOV_MAX);
#endif
const std::size_t max_tmp_name_size( 512);
// #ifdef BOOST_NET_IPV4
const std::size_t addrstrlen_ipv4( INET_ADDRSTRLEN);
// #endif
// #ifdef BOOST_NET_IPV6
const std::size_t addrstrlen_ipv6( INET6_ADDRSTRLEN);
// #endif
} }

#endif // BOOST_NET_CONSTANTS_H

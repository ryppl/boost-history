//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV6_UTILITY_H
#define BOOST_NET_IPV6_UTILITY_H

extern "C"
{
#include <netinet/in.h>
}

#include <boost/mpl/int.hpp>

namespace boost { namespace net {
namespace ipv6
{
enum socket_type
{
	socket_dgram = SOCK_DGRAM,
	socket_raw = SOCK_RAW,
	socket_stream = SOCK_STREAM,
	socket_seqpacket = SOCK_SEQPACKET,
	socket_unspec = 0
};

enum protocol_type
{
	protocol_icmp = IPPROTO_ICMP,
	protocol_sctp = IPPROTO_SCTP,
	protocol_tcp = IPPROTO_TCP,
	protocol_udp = IPPROTO_UDP,
	protocol_unspec = 0
};

enum resolve_flags
{
	canonical_name = AI_CANONNAME,
	v4mapped = AI_V4MAPPED,
	v4mapped_all = AI_ALL
};
}}}

#endif // BOOST_NET_IPV6_UTILITY_H

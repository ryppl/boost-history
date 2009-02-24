//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_UTILITY_H
#define BOOST_NET_IPV4_UTILITY_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
#include <netinet/ip.h>
}

namespace boost { namespace net {
namespace ipv4
{
enum family_type
{
	family_unspecified = PF_UNSPEC,
	family_ipv4 = PF_INET
};

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
	canonical_name = AI_CANONNAME
};

const int low_delay = IPTOS_LOWDELAY;
const int throughput = IPTOS_THROUGHPUT;
#ifdef BOOST_NET_HAVE_IPTOS_LOWCOST
const int low_cost = IPTOS_LOWCOST;
#endif
const int reliability = IPTOS_RELIABILITY;
const int mincost = IPTOS_MINCOST;
}}}

#endif // BOOST_NET_IPV4_UTILITY_H

//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV6_ADDRESS_INFO_H
#define BOOST_NET_IPV6_ADDRESS_INFO_H

#include <string>

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
}

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/net/ipv6/address.hpp>
#include <boost/net/ipv6/utility.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv6
{
class address_info
{
private:
	shared_ptr< addrinfo_type >	root_;
	addrinfo_type			*	current_;

public:
	address_info(
		shared_ptr< addrinfo_type >	root,
		addrinfo_type * current)
	: root_( root), current_( current)
	{
		BOOST_ASSERT( root_);
		BOOST_ASSERT( current_);
	}

	const std::string canonical_name() const
	{
		BOOST_ASSERT( root_);
		return root_->ai_canonname ? root_->ai_canonname : "";
	}

	socket_type socktype() const
	{
		BOOST_ASSERT( root_);
		BOOST_ASSERT( current_);
		return static_cast< socket_type >( current_->ai_socktype);
	}

	protocol_type protocol() const
	{
		BOOST_ASSERT( root_);
		BOOST_ASSERT( current_);
		return static_cast< protocol_type >( current_->ai_protocol);
	}

	const address to_address() const
	{
		BOOST_ASSERT( root_);
		BOOST_ASSERT( current_);

		void * vp( current_->ai_addr);
		sockaddr_in6_type * in6( static_cast< sockaddr_in6_type * >( vp) );
		return address( * in6);
	}

	operator const address() const
	{ return to_address(); }
};
}}}

#endif // BOOST_NET_IPV6_ADDRESS_INFO_H

//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_TCP_DETAIL_OPTIONS_H
#define BOOST_NET_IPV4_TCP_DETAIL_OPTIONS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/tcp.h>
}

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace tcp {
namespace detail
{
class options
{
private:
	net::detail::socket	socket_;

public:
	options( net::detail::socket const& socket__)
	: socket_( socket__)
	{}

	bool no_delay() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( 0);
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_TCP,
			TCP_NODELAY,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void no_delay( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_TCP,
			TCP_NODELAY,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t max_segment_size() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			IPPROTO_TCP,
			TCP_MAXSEG,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void max_segment_size( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );

		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_TCP,
			TCP_MAXSEG,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
};
}}}}}

#endif // BOOST_NET_IPV4_TCP_DETAIL_OPTIONS_H

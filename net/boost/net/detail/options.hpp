//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_DETAIL_OPTIONS_H
#define BOOST_NET_DETAIL_OPTIONS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <strings.h>
#include <sys/socket.h>
}

#include <cstddef>
#include <cmath>
#include <string>

#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace detail
{
class options
{
private:
	socket	socket_;

public:
	options( socket const& socket__)
	: socket_( socket__)
	{}

	bool broadcast() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( 0);
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_BROADCAST,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void broadcast( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_BROADCAST,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool keep_alive() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_KEEPALIVE,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void keep_alive( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_KEEPALIVE,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool is_debug() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_DEBUG,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void debug( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_DEBUG,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool dont_route() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( 0);
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		net::detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_DONTROUTE,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void dont_route( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_DONTROUTE,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool linger( posix_time::time_duration & timeout) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		linger_type value;
		sockaddr_len_type len( sizeof( linger_type) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_LINGER,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		timeout = posix_time::seconds( value.l_linger);
		return value.l_onoff != 0;
	}

	void linger( bool v, posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		linger_type value;
		value.l_onoff = v ? 1 : 0;
		value.l_linger = timeout.total_seconds();
		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_LINGER,
			& value,
			sizeof( linger_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t receive_buffer() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_RCVBUF,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void receive_buffer( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_RCVBUF,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t receive_lowat() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_RCVLOWAT,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void receive_lowat( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_RCVLOWAT,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const posix_time::time_duration receive_timeout() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		timeval_type value;
		sockaddr_len_type len( sizeof( timeval_type) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_RCVTIMEO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return posix_time::seconds( value.tv_sec) + posix_time::microseconds( value.tv_usec);
	}

	void receive_timeout( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		timeval_type value;
		value.tv_sec = timeout.total_seconds();
		value.tv_usec = numeric_cast< long >(
			timeout.fractional_seconds() * ::pow( 10, 6 - timeout.num_fractional_digits() ) );
		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_RCVTIMEO,
			& value,
			sizeof( timeval_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool reuse_address() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( 0);
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_REUSEADDR,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void reuse_address( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_REUSEADDR,
			& value,
			sizeof( int),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
	std::size_t send_buffer() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_SNDBUF,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void send_buffer( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_SNDBUF,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t send_lowat() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_SNDLOWAT,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void send_lowat( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( value > 0);

		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_SNDLOWAT,
			& value,
			sizeof( std::size_t),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const posix_time::time_duration send_timeout() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		timeval_type value;
		sockaddr_len_type len( sizeof( timeval_type) );
		system::error_code ec;
		detail::getsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_SNDTIMEO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return posix_time::seconds( value.tv_sec) + posix_time::microseconds( value.tv_usec);
	}

	void send_timeout( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		timeval_type value;
		value.tv_sec = timeout.total_seconds();
		value.tv_usec = numeric_cast< long >(
			timeout.fractional_seconds() * ::pow( 10, 6 - timeout.num_fractional_digits() ) );
		system::error_code ec;
		detail::setsockopt(
			socket_.native_handle(),
			SOL_SOCKET,
			SO_SNDTIMEO,
			& value,
			sizeof( timeval_type),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
};

}}}

#endif // BOOST_NET_DETAIL_OPTIONS_H

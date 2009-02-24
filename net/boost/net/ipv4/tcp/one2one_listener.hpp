//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_TCP_ONE2ONE_LISTENER_H
#define BOOST_NET_IPV4_TCP_ONE2ONE_LISTENER_H

#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/options.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/ipv4/address.hpp>
#include <boost/net/ipv4/tcp/one2one.hpp>
#include <boost/net/ipv4/utility.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace tcp
{
class one2one_listener
:
private base_from_member< net::detail::socket >,
private net::detail::options
{
private:
	typedef base_from_member< net::detail::socket >  socket_base_type;

public:
//------------------------------------------
	one2one_listener()
	:
	socket_base_type( family_ipv4, socket_stream, protocol_tcp),
	net::detail::options( member)
	{}

//------------------------------------------

	native_handle_type native_handle() const
	{ return member.native_handle(); }

	bool is_valid() const
	{ return member.is_valid(); }

	void close()
	{ member.close(); }

	const system::error_code error() const
	{
		BOOST_ASSERT( is_valid() );

		return member.error();
	}

	bool is_non_blocking() const
	{
		BOOST_ASSERT( is_valid() );

		return member.is_non_blocking();
	}

	void non_blocking( bool value)
	{
		BOOST_ASSERT( is_valid() );

		member.non_blocking( value);
	}

//------------------------------------------

	void shutdown( shutdown_mode const& mode)
	{
		BOOST_ASSERT( is_valid() );

		system::error_code ec;
		net::detail::shutdown( native_handle(), mode, ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void bind( address const& addr)
	{
		BOOST_ASSERT( is_valid() );

		system::error_code ec;
		net::detail::bind(
			native_handle(),
			addr.sockaddr_ptr(),
			addr.size(),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void listen( int backlog = default_backlog_size)
	{
		BOOST_ASSERT( is_valid() );

		char * p( 0);
		if ( ( p = net::detail::getenv("LISTENQ") ) != 0)
			backlog = lexical_cast< int >( p);

		BOOST_ASSERT( backlog > 0);

		system::error_code ec;
		net::detail::listen( native_handle(), backlog, ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	bool accept( one2one & strm)
	{
		BOOST_ASSERT( is_valid() );
	
		system::error_code ec;
		native_handle_type handle( invalid_native_handle);
		do
		{
			ec.clear();
			handle = net::detail::accept(
				native_handle(),
				0,
				0,
				ec);
		}
		while (
			ec.value() == system::errc::interrupted ||
			ec.value() == system::errc::connection_aborted ||
			ec.value() == system::errc::protocol_error);
	
		if ( ec)
		{
			if ( ec.value() == system::errc::operation_would_block ||
				ec.value() == system::errc::resource_unavailable_try_again)
				return false;
			else
				throw_exception( system::system_error( ec) );
		}

		strm = one2one( handle);
		strm.non_blocking( is_non_blocking() );
		return true;
	}

	bool accept(
		tcp::one2one & strm,
		posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( is_valid() );
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );
	
		system::error_code ec;
		net::detail::is_readable( native_handle(), timeout, ec);
		if ( ec) throw_exception( system::system_error( ec) );
	
		return accept( strm);
	}

//------------------------------------------

	const address local_address() const
	{
		BOOST_ASSERT( is_valid() );

		system::error_code ec;
		sockaddr_in4_type sa;
		sockaddr_len_type len( sizeof( sockaddr_in4_type) );
		void * vp( & sa);
		net::detail::getsockname(
			native_handle(),
			static_cast< sockaddr_type * >( vp),
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return address( sa);
	}

//------------------------------------------

	using net::detail::options::reuse_address;
};
}}}}

#endif // BOOST_NET_IPV4_TCP_ONE2ONE_LISTENER_H

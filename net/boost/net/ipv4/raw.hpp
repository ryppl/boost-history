//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_RAW_H
#define BOOST_NET_IPV4_RAW_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/in.h>
#include <sys/socket.h>
}

#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/constants.hpp>
#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/options.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/ipv4/address.hpp>
#include <boost/net/ipv4/detail/socket.hpp>
#include <boost/net/ipv4/detail/options.hpp>
#include <boost/net/ipv4/utility.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4
{
template< int Protocol >
class raw
:
private base_from_member< net::detail::socket >,
private net::detail::options,
private ipv4::detail::options
{
private:
	typedef base_from_member< net::detail::socket >  socket_base_type;

public:
//------------------------------------------
	raw()
	:
	socket_base_type( family_ipv4, socket_raw, Protocol),
	net::detail::options( member),
	ipv4::detail::options( member)
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

		sockaddr_in4_type const& sa( ep.as_sockaddr_in4() );
		sockaddr_len_type len( sizeof sa);
		system::error_code ec;
		net::detail::bind(
			native_handle(),
			addr.sockaddr_ptr(),
			addr.size(),
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

//------------------------------------------

	using net::detail::options::dont_route;
	using net::detail::options::linger;
	using net::detail::options::receive_buffer;
	using net::detail::options::receive_lowat;
	using net::detail::options::receive_timeout;
	using net::detail::options::send_buffer;
	using net::detail::options::send_lowat;
	using net::detail::options::send_timeout;

//------------------------------------------

	using ipv4::detail::options::time_to_live;
	using ipv4::detail::options::type_of_service;
	using ipv4::detail::options::include_header;
	using ipv4::detail::options::ip_options;
};
}}}

#endif // BOOST_NET_IPV4_RAW_H

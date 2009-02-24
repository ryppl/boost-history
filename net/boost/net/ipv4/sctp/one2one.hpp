//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_ONE2ONE_H
#define BOOST_NET_IPV4_SCTP_ONE2ONE_H

#include <cstddef>
#include <cstring>
#include <vector>

#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/options.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/ipv4/address.hpp>
#include <boost/net/ipv4/detail/operations.hpp>
#include <boost/net/ipv4/detail/options.hpp>
#include <boost/net/ipv4/sctp/detail/options.hpp>
#include <boost/net/ipv4/utility.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
class one2one
:
private base_from_member< net::detail::socket >,
private net::detail::options,
private ipv4::detail::options,
private detail::options
{
private:
	typedef base_from_member< net::detail::socket >  socket_base_type;

	friend class one2one_listener;
	friend class one2many;
	friend class one2many_listener;

	one2one( native_handle_type handle)
	:
	socket_base_type( handle),
	net::detail::options( member),
	ipv4::detail::options( member),
	detail::options( member)
	{}

public:
//------------------------------------------
	one2one()
	:
	socket_base_type( family_ipv4, socket_stream, protocol_sctp),
	net::detail::options( member),
	ipv4::detail::options( member),
	detail::options( member)
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

	bool connect( address const& addr)
	{
		system::error_code ec;
		detail::sctp_connectx(
			native_handle(),
			addr.sockaddr_ptr(),
			1,
			ec);
		if ( ec)
		{
			if ( ec.value() == system::errc::interrupted ||
				ec.value() == system::errc::operation_in_progress)
				return false;
			else
				throw_exception( system::system_error( ec) );
		}
		return true;
	}

	bool connect(
		address const& addr,
		posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );
	
		system::error_code ec;
		net::detail::is_readable( native_handle(), timeout, ec);
		if ( ec) throw_exception( system::system_error( ec) );
	
		return connect( addr);
	}

	bool connect( std::vector< address > const& addrs)
	{
		std::vector< sockaddr_in4_type > tmp;
		tmp.reserve( addrs.size() );
		void * vp( & tmp[0]);
		char * p( static_cast< char * >( vp) );
		std::vector< address >::const_iterator e( addrs.end() );
		for (
			std::vector< address >::const_iterator i( addrs.begin() );
			i != e;
			++i)
		{
			std::memcpy( p, i->sockaddr_ptr(), i->size() );
			p += i->size();
		}
		system::error_code ec;
		detail::sctp_connectx(
			native_handle(),
			static_cast< sockaddr_type * >( vp),
			addrs.size(),
			ec);
		if ( ec)
		{
			if ( ec.value() == system::errc::interrupted ||
				ec.value() == system::errc::operation_in_progress)
				return false;
			else
				throw_exception( system::system_error( ec) );
		}
		return true;
	}

	bool connect(
		std::vector< address > const& addrs,
		posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );
	
		system::error_code ec;
		net::detail::is_readable( native_handle(), timeout, ec);
		if ( ec) throw_exception( system::system_error( ec) );
	
		return connect( addrs);
	}

//------------------------------------------

	const std::vector< address > local_addresses() const
	{
		BOOST_ASSERT( is_valid() );

		sockaddr_type * addr( 0);
		system::error_code ec;
		std::size_t n(
			detail::sctp_getladdrs(
				native_handle(),
				static_cast< sctp_assoc_type >( 0),
				& addr,
				ec) );
		if ( ec) throw_exception( system::system_error( ec) );
		void * vp( addr);
		std::vector< address > addrs;
		addrs.reserve( n);
		for ( std::size_t i( 0); i < n; ++i)
		{
			addrs.push_back( * static_cast< sockaddr_in4_type * >( vp) );
			char * cp( static_cast< char * >( vp) );
			cp += sizeof( sockaddr_in4_type);
			vp = cp;
		}
		detail::sctp_freeladdrs( addr);
		return addrs;
	}

	const std::vector< address > peer_addresses() const
	{
		BOOST_ASSERT( is_valid() );

		sockaddr_type * addr( 0);
		system::error_code ec;
		std::size_t n(
			detail::sctp_getpaddrs(
				native_handle(),
				static_cast< sctp_assoc_type >( 0),
				& addr,
				ec) );
		if ( ec) throw_exception( system::system_error( ec) );
		void * vp( addr);
		std::vector< address > addrs;
		addrs.reserve( n);
		for ( std::size_t i( 0); i < n; ++i)
		{
			addrs.push_back( * static_cast< sockaddr_in4_type * >( vp) );
			char * cp( static_cast< char * >( vp) );
			cp += sizeof( sockaddr_in4_type);
			vp = cp;
		}
		detail::sctp_freepaddrs( addr);
		return addrs;
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

//------------------------------------------

	using detail::options::adaptation_layer;
	using detail::options::association_info;
	using detail::options::association_status;
	using detail::options::default_send_parameters;
	using detail::options::disable_autoclose;
	using detail::options::heartbeat;
	using detail::options::initmsg;
	using detail::options::local_primary_address;
#ifdef BOOST_NET_HAVE_SCTP_MAXSEG
	using detail::options::max_seg;
#endif
#ifdef BOOST_NET_HAVE_SCTP_MAXBURST
	using detail::options::max_burst
#endif
	using detail::options::no_delay;
	using detail::options::no_fragments;
	using detail::options::peer_address_params;
	using detail::options::peer_primary_address;
	using detail::options::rto_info;
	using detail::options::subscriptions;
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_ONE2ONE_H

//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_ONE2MANY_H
#define BOOST_NET_IPV4_SCTP_ONE2MANY_H

#include <cstddef>
#include <vector>

#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/options.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/ipv4/address.hpp>
#include <boost/net/ipv4/detail/operations.hpp>
#include <boost/net/ipv4/detail/options.hpp>
#include <boost/net/ipv4/sctp/association.hpp>
#include <boost/net/ipv4/sctp/detail/association_options.hpp>
#include <boost/net/ipv4/sctp/detail/options.hpp>
#include <boost/net/ipv4/sctp/one2one.hpp>
#include <boost/net/ipv4/utility.hpp>
#include <boost/net/shutdown_mode.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
class one2many
:
private base_from_member< net::detail::socket >,
private net::detail::options,
private ipv4::detail::options,
private detail::association_options,
private detail::options
{
private:
	typedef base_from_member< net::detail::socket >  socket_base_type;

public:
//------------------------------------------

	one2many()
	:
	socket_base_type( family_ipv4, socket_seqpacket, protocol_sctp),
	net::detail::options( member),
	ipv4::detail::options( member),
	detail::association_options( member),
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

	const one2one peel_off( association const& assoc)
	{
		system::error_code ec;
		one2one strm( detail::sctp_peeloff( native_handle(), assoc, ec) );
		if ( ec) throw_exception( system::system_error( ec) );
		return strm;
	}

	const association find_association( address const& addr) const
	{
		BOOST_ASSERT( is_valid() );

		return peer_address_params( addr).assoc();
	}

//------------------------------------------

	const std::vector< address > local_addresses( association const& assoc) const
	{
		BOOST_ASSERT( is_valid() );

		sockaddr_type * addr( 0);
		system::error_code ec;
		std::size_t n(
			detail::sctp_getladdrs(
				native_handle(),
				assoc,
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

	const std::vector< address > peer_addresses( association const& assoc) const
	{
		BOOST_ASSERT( is_valid() );

		sockaddr_type * addr( 0);
		system::error_code ec;
		std::size_t n(
			detail::sctp_getpaddrs(
				native_handle(),
				assoc,
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
	using detail::options::autoclose;
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

	
	using detail::association_options::association_info;
	using detail::association_options::association_status;
	using detail::association_options::default_send_parameters;
	using detail::association_options::heartbeat;
	using detail::association_options::local_primary_address;
	using detail::association_options::peer_primary_address;
	using detail::association_options::rto_info;
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_ONE2MANY_H

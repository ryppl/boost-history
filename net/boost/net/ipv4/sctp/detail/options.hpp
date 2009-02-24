//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_DETAIL_OPTIONS_H
#define BOOST_NET_IPV4_SCTP_DETAIL_OPTIONS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/sctp.h>
#include <strings.h>
}

#include <cstring>
#include <string>

#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility.hpp>

#include <boost/net/detail/operations.hpp>
#include <boost/net/detail/socket.hpp>
#include <boost/net/ipv4/address.hpp>
#include <boost/net/ipv4/sctp/address_params.hpp>
#include <boost/net/ipv4/sctp/association_params.hpp>
#include <boost/net/ipv4/sctp/association_state.hpp>
#include <boost/net/ipv4/sctp/default_send_params.hpp>
#include <boost/net/ipv4/sctp/detail/operations.hpp>
#include <boost/net/ipv4/sctp/heartbeat_params.hpp>
#include <boost/net/ipv4/sctp/initmsg_params.hpp>
#include <boost/net/ipv4/sctp/rto_params.hpp>
#include <boost/net/ipv4/sctp/subscription_params.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp {
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
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_NODELAY,
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
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_NODELAY,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
#ifdef BOOST_NET_HAVE_SCTP_MAXSEG
	std::size_t max_seg() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_MAX_SEG,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void max_seg( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_MAX_SEG,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
#endif
#ifdef BOOST_NET_HAVE_SCTP_MAXBURST
	std::size_t max_burst() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_MAXBURST,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value;
	}

	void max_burst( std::size_t value)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_MAXBURST,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
#endif
	bool no_fragments() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( 0);
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_DISABLE_FRAGMENTS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value != 0;
	}

	void no_fragments( bool value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		int value( value_ ? 1 : 0);
		sockaddr_len_type len( sizeof( int) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_DISABLE_FRAGMENTS,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const posix_time::time_duration autoclose() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_AUTOCLOSE,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return posix_time::seconds( value);
	}

	void autoclose( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( socket_.is_valid() );
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		std::size_t value( timeout.total_seconds() );
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_AUTOCLOSE,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void disable_autoclose()
	{
		BOOST_ASSERT( socket_.is_valid() );

		std::size_t value( 0);
		sockaddr_len_type len( sizeof( std::size_t) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_AUTOCLOSE,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	std::size_t adaptation_layer() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setadaptation_type value;
		sockaddr_len_type len( sizeof( sctp_setadaptation_type) );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_ADAPTATION_LAYER,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return value.ssb_adaptation_ind;
	}

	void adaptation_layer( std::size_t value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setadaptation_type value;
		sockaddr_len_type len( sizeof( sctp_setadaptation_type) );
		value.ssb_adaptation_ind = value_;
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_ADAPTATION_LAYER,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void peer_primary_address( address const& addr)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setpeerprim_type value;
		sockaddr_len_type len( sizeof( sctp_setpeerprim_type) );
		::bzero( & value, len);
		std::memcpy( & value.sspp_addr, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_SET_PEER_PRIMARY_ADDR,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const address local_primary_address() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setprim_type value;
		sockaddr_len_type len( sizeof( sctp_setprim_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_PRIMARY_ADDR,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		void const* vp( & value.ssp_addr);
		return address( * static_cast< sockaddr_in4_type const* >( vp) );
	}

	void local_primary_address( address const& addr)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setprim_type value;
		sockaddr_len_type len( sizeof( sctp_setprim_type) );
		::bzero( & value, len);
		std::memcpy( & value.ssp_addr, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_PRIMARY_ADDR,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const heartbeat_params heartbeat( address const& addr) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value;
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		::bzero( & value, len);
		std::memcpy( & value.spp_address, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_PEER_ADDR_PARAMS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return heartbeat_params( value);
	}

	const heartbeat_params heartbeat() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value;
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		::bzero( & value, len);;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_PEER_ADDR_PARAMS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return heartbeat_params( value);
	}

	void heartbeat( address const& addr, heartbeat_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value( params);
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		std::memcpy( & value.spp_address, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_PEER_ADDR_PARAMS,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	void heartbeat( heartbeat_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value( params);
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_PEER_ADDR_PARAMS,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const rto_params rto_info() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_rtoinfo_type value;
		sockaddr_len_type len( sizeof( sctp_rtoinfo_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_RTOINFO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return rto_params( value);
	}

	void rto_info( rto_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_rtoinfo_type value( params);
		sockaddr_len_type len( sizeof( sctp_rtoinfo_type) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_RTOINFO,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const initmsg_params initmsg() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_initmsg_type value;
		sockaddr_len_type len( sizeof( sctp_initmsg_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_INITMSG,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return initmsg_params( value);
	}

	void initmsg( initmsg_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_initmsg_type value( params);
		sockaddr_len_type len( sizeof( sctp_initmsg_type) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_INITMSG,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const subscription_params subscriptions() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_event_subscribe_type value;
		sockaddr_len_type len( sizeof( sctp_event_subscribe_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_EVENTS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return subscription_params( value);
	}

	void subscriptions( subscription_params const& value_)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_event_subscribe_type value( value_);
		sockaddr_len_type len( sizeof( sctp_event_subscribe_type) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_INITMSG,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const address_params peer_address_params( address const& addr) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrinfo_type value;
		sockaddr_len_type len( sizeof( sctp_paddrinfo_type) );
		::bzero( & value, len);
		std::memcpy( & value.spinfo_address, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_GET_PEER_ADDR_INFO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return address_params( value);
	}

	const association_state association_status() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_status_type value;
		sockaddr_len_type len( sizeof( sctp_status_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_STATUS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return association_state( value);
	}

	const association_params association_info() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_assocparams_type value;
		sockaddr_len_type len( sizeof( sctp_assocparams_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_ASSOCINFO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return association_params( value);
	}

	void association_info( association_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_assocparams_type value( params);
		sockaddr_len_type len( sizeof( sctp_assocparams_type) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_ASSOCINFO,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}

	const default_send_params default_send_parameters() const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_sndrcvinfo_type value;
		sockaddr_len_type len( sizeof( sctp_sndrcvinfo_type) );
		::bzero( & value, len);
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			0,
			SCTP_DEFAULT_SEND_PARAM,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return default_send_params( value);
	}

	void default_send_parameters( default_send_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_sndrcvinfo_type value( params);
		sockaddr_len_type len( sizeof( sctp_sndrcvinfo_type) );
		system::error_code ec;
		net::detail::setsockopt(
			socket_.native_handle(),
			IPPROTO_SCTP,
			SCTP_DEFAULT_SEND_PARAM,
			& value,
			len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
	}
};
}}}}}

#endif // BOOST_NET_IPV4_SCTP_DETAIL_OPTIONS_H

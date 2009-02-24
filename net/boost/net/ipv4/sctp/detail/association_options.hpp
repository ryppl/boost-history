//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_DETAIL_ASSOCIATION_OPTIONS_H
#define BOOST_NET_IPV4_SCTP_DETAIL_ASSOCIATION_OPTIONS_H

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
#include <boost/net/ipv4/sctp/association.hpp>
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
class association_options
{
private:
	net::detail::socket	socket_;

public:
	association_options( net::detail::socket const& socket__)
	: socket_( socket__)
	{}

	const association_state association_status( association const& assoc) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_status_type value;
		sockaddr_len_type len( sizeof( sctp_status_type) );
		::bzero( & value, len);
		value.sstat_assoc_id = assoc;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_STATUS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return association_state( value);
	}

	void peer_primary_address( association const& assoc, address const& addr)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setpeerprim_type value;
		sockaddr_len_type len( sizeof( sctp_setpeerprim_type) );
		::bzero( & value, len);
		value.sspp_assoc_id = assoc;
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

	const address local_primary_address( association const& assoc) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setprim_type value;
		sockaddr_len_type len( sizeof( sctp_setprim_type) );
		::bzero( & value, len);
		value.ssp_assoc_id = assoc;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_PRIMARY_ADDR,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		void const* vp( & value.ssp_addr);
		return address( * static_cast< sockaddr_in4_type const* >( vp) );
	}

	void local_primary_address( association const& assoc, address const& addr)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_setprim_type value;
		sockaddr_len_type len( sizeof( sctp_setprim_type) );
		::bzero( & value, len);
		value.ssp_assoc_id = assoc;
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

	const heartbeat_params heartbeat( association const& assoc, address const& addr) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value;
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		::bzero( & value, len);
		value.spp_assoc_id = assoc;
		std::memcpy( & value.spp_address, addr.sockaddr_ptr(), addr.size() );
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_PEER_ADDR_PARAMS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return heartbeat_params( value);
	}

	const heartbeat_params heartbeat( association const& assoc) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value;
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		::bzero( & value, len);
		value.spp_assoc_id = assoc;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_PEER_ADDR_PARAMS,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return heartbeat_params( value);
	}

	void heartbeat( association const& assoc, address const& addr, heartbeat_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value( params);
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		value.spp_assoc_id = assoc;
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

	void heartbeat( association const& assoc, heartbeat_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_paddrparams_type value( params);
		sockaddr_len_type len( sizeof( sctp_paddrparams_type) );
		value.spp_assoc_id = assoc;
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

	const rto_params rto_info( association const& assoc) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_rtoinfo_type value;
		sockaddr_len_type len( sizeof( sctp_rtoinfo_type) );
		::bzero( & value, len);
		value.srto_assoc_id = assoc;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_RTOINFO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return rto_params( value);
	}

	void rto_info( association const& assoc, rto_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_rtoinfo_type value( params);
		sockaddr_len_type len( sizeof( sctp_rtoinfo_type) );
		value.srto_assoc_id = assoc;
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

	const association_params association_info( association const& assoc) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_assocparams_type value;
		sockaddr_len_type len( sizeof( sctp_assocparams_type) );
		::bzero( & value, len);
		value.sasoc_assoc_id = assoc;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_ASSOCINFO,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return association_params( value);
	}

	void association_info( association const& assoc, association_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_assocparams_type value( params);
		sockaddr_len_type len( sizeof( sctp_assocparams_type) );
		value.sasoc_assoc_id = assoc;
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

	const default_send_params default_send_parameters( association const& assoc) const
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_sndrcvinfo_type value;
		sockaddr_len_type len( sizeof( sctp_sndrcvinfo_type) );
		::bzero( & value, len);
		value.sinfo_assoc_id = assoc;
		system::error_code ec;
		detail::sctp_opt_info(
			socket_.native_handle(),
			assoc,
			SCTP_DEFAULT_SEND_PARAM,
			& value,
			& len,
			ec);
		if ( ec) throw_exception( system::system_error( ec) );
		return default_send_params( value);
	}

	void default_send_parameters( association const& assoc, default_send_params const& params)
	{
		BOOST_ASSERT( socket_.is_valid() );

		sctp_sndrcvinfo_type value( params);
		sockaddr_len_type len( sizeof( sctp_sndrcvinfo_type) );
		value.sinfo_assoc_id = assoc;
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

#endif // BOOST_NET_IPV4_SCTP_DETAIL_ASSOCIATION_OPTIONS_H

//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_ASSOCIATION_STATE_H
#define BOOST_NET_IPV4_SCTP_ASSOCIATION_STATE_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/sctp.h>
}

#include <cstddef>

#include <boost/net/ipv4/address.hpp>
#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
namespace detail
{
	class options;
	class association_options;
}

class association_state
{
private:
	friend class detail::options;
	friend class detail::association_options;

	sctp_status_type		ss_;

	association_state( sctp_status_type ss)
	: ss_( ss)
	{}

public:
	bool is_closed() const
	{ return ss_.sstat_state == SCTP_CLOSED; }

	bool has_send_init() const
	{ return ss_.sstat_state == SCTP_COOKIE_WAIT; }

	bool has_echoed_cookie() const
	{ return ss_.sstat_state == SCTP_COOKIE_ECHOED; }

	bool is_established() const
	{ return ss_.sstat_state == SCTP_ESTABLISHED; }

	bool is_shutdown_pending() const
	{ return ss_.sstat_state == SCTP_SHUTDOWN_PENDING; }

	bool has_shutdown_send() const
	{ return ss_.sstat_state == SCTP_SHUTDOWN_SENT; }

	bool has_shutdown_received() const
	{ return ss_.sstat_state == SCTP_SHUTDOWN_RECEIVED; }

	bool is_waiting_shutdown_complete() const
	{ return ss_.sstat_state == SCTP_SHUTDOWN_ACK_SENT; }

	std::size_t peer_receive_window() const
	{ return ss_.sstat_rwnd; }

	std::size_t peer_nack_data() const
	{ return ss_.sstat_unackdata; }

	std::size_t peer_send_streams() const
	{ return ss_.sstat_instrms; }

	std::size_t local_pending_data() const
	{ return ss_.sstat_penddata; }

	std::size_t local_send_streams() const
	{ return ss_.sstat_outstrms; }

	std::size_t local_fragmentation_point() const
	{ return ss_.sstat_fragmentation_point; }

	const address local_primary_address() const
	{
		void const* vp( & ss_.sstat_primary);
		return address( * static_cast< sockaddr_in4_type const* >( vp) );
	}
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_ASSOCIATION_STATE_H

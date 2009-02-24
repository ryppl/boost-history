//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_ADDRESS_PARAMS_H
#define BOOST_NET_IPV4_SCTP_ADDRESS_PARAMS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/sctp.h>
}

#include <cstddef>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
namespace detail
{ class options; }

class address_params
{
private:
	friend class detail::options;

	sctp_paddrinfo_type		sp_;

	address_params( sctp_paddrinfo_type sp)
	: sp_( sp)
	{}

public:
	const association assoc() const
	{ return association( sp_.spinfo_assoc_id); }

	bool is_active() const
	{ return sp_.spinfo_state == SCTP_ACTIVE; }

	bool is_incative() const
	{ return sp_.spinfo_state == SCTP_INACTIVE; }

	bool is_address_unconfirmed() const
	{ return sp_.spinfo_state == SCTP_UNCONFIRMED; }

	std::size_t congestion_window() const
	{ return sp_.spinfo_cwnd; }

	std::size_t smoothed_rtt() const
	{ return sp_.spinfo_srtt; }

	const posix_time::time_duration retransmission_timeout() const
	{ return posix_time::milliseconds( sp_.spinfo_rto); }

	std::size_t path_mtu() const
	{ return sp_.spinfo_mtu; }
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_ADDRESS_PARAMS_H

//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_SUBSCRIPTION_PARAMS_H
#define BOOST_NET_IPV4_SCTP_SUBSCRIPTION_PARAMS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <strings.h>
}

#include <cstddef>

#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
namespace detail
{ class options; }

class subscription_params
{
private:
	friend class detail::options;

	sctp_event_subscribe_type	se_;

	subscription_params( sctp_event_subscribe_type se)
	: se_( se)
	{}

	operator sctp_event_subscribe_type const () const
	{ return se_; }

public:
	subscription_params()
	: se_()
	{ ::bzero( & se_, sizeof( sctp_event_subscribe_type) ); }

	bool is_data_io() const
	{ return se_.sctp_data_io_event == 1; }

	void data_io( bool value)
	{ se_.sctp_data_io_event = value ? 1 : 0; }

	bool is_association() const
	{ return se_.sctp_association_event == 1; }

	void association( bool value)
	{ se_.sctp_association_event = value ? 1 : 0; }

	bool is_address() const
	{ return se_.sctp_address_event == 1; }

	void address( bool value)
	{ se_.sctp_address_event = value ? 1 : 0; }

	bool is_send_failure() const
	{ return se_.sctp_send_failure_event == 1; }

	void send_failure( bool value)
	{ se_.sctp_send_failure_event = value ? 1 : 0; }

	bool is_peer_error() const
	{ return se_.sctp_peer_error_event == 1; }

	void peer_error( bool value)
	{ se_.sctp_peer_error_event = value ? 1 : 0; }

	bool is_shutdown() const
	{ return se_.sctp_shutdown_event == 1; }

	void shutdown( bool value)
	{ se_.sctp_shutdown_event = value ? 1 : 0; }

	bool is_partial_delivery() const
	{ return se_.sctp_partial_delivery_event == 1; }

	void partial_delivery( bool value)
	{ se_.sctp_partial_delivery_event = value ? 1 : 0; }

	bool is_adaptation_layer() const
	{ return se_.sctp_adaptation_layer_event == 1; }

	void adaptation_layer( bool value)
	{ se_.sctp_adaptation_layer_event = value ? 1 : 0; }
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_SUBSCRIPTION_PARAMS_H

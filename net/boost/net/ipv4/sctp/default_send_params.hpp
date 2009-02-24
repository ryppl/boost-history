//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_DEFAULT_SEND_PARAMS_H
#define BOOST_NET_IPV4_SCTP_DEFAULT_SEND_PARAMS_H

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
{
	class options;
	class association_options;
}

class default_send_params
{
private:
	friend class detail::options;
	friend class detail::association_options;

	sctp_sndrcvinfo_type		sp_;

	default_send_params( sctp_sndrcvinfo_type sp)
	: sp_( sp)
	{}

	operator sctp_sndrcvinfo_type const () const
	{ return sp_; }

public:
	default_send_params()
	: sp_()
	{ ::bzero( & sp_, sizeof( sctp_sndrcvinfo_type) ); }

	std::size_t default_stream() const
	{ return sp_.sinfo_stream; }

	void default_stream( std::size_t value)
	{ sp_.sinfo_stream = value; }

	std::size_t stream_sequence_number() const
	{ return sp_.sinfo_ssn; }

	void stream_sequence_number( std::size_t value)
	{ sp_.sinfo_ssn = value; }

	std::size_t payload_protocol_identifier() const
	{ return sp_.sinfo_ppid; }

	void payload_protocol_identifier( std::size_t value)
	{ sp_.sinfo_ppid = value; }

	std::size_t context() const
	{ return sp_.sinfo_context; }

	void context( std::size_t value)
	{ sp_.sinfo_context = value; }

	std::size_t time_to_live() const
	{ return sp_.sinfo_timetolive; }

	void time_to_live( std::size_t value)
	{ sp_.sinfo_timetolive = value; }

	std::size_t transport_sequence_number() const
	{ return sp_.sinfo_tsn; }

	void transport_sequence_number( std::size_t value)
	{ sp_.sinfo_tsn = value; }

	std::size_t cumulative_transport_sequence_number() const
	{ return sp_.sinfo_cumtsn; }

	void cumulative_transport_sequence_number( std::size_t value)
	{ sp_.sinfo_cumtsn = value; }
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_DEFAULT_SEND_PARAMS_H

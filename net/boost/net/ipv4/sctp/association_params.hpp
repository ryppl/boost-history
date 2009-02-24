//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_ASSOCIATION_PARAMS_H
#define BOOST_NET_IPV4_SCTP_ASSOCIATION_PARAMS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <strings.h>
}

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
namespace detail
{
	class options;
	class association_options;
}

class association_params
{
private:
	friend class detail::options;
	friend class detail::association_options;

	sctp_assocparams_type	sa_;

	association_params( sctp_assocparams_type sa)
	: sa_( sa)
	{}

	operator const sctp_assocparams_type () const
	{ return sa_; }

public:
	association_params()
	: sa_()
	{ ::bzero( & sa_, sizeof( sctp_assocparams_type) ); }

	std::size_t max_retransmissions() const
	{ return sa_.sasoc_asocmaxrxt; }

	void max_retransmissions( std::size_t value)
	{
		BOOST_ASSERT( value >= 0);

		sa_.sasoc_asocmaxrxt = value;
	}

	std::size_t peer_destinations() const
	{ return sa_.sasoc_number_peer_destinations; }

	std::size_t peer_receive_window() const
	{ return sa_.sasoc_peer_rwnd; }

	std::size_t local_receive_window() const
	{ return sa_.sasoc_local_rwnd; }

	const posix_time::time_duration cookie_life() const
	{ return posix_time::milliseconds( sa_.sasoc_cookie_life); }

	void cookie_life( posix_time::time_duration const& lifetime)
	{
		BOOST_ASSERT( ! lifetime.is_special() );
		BOOST_ASSERT( ! lifetime.is_negative() );

		sa_.sasoc_cookie_life = lifetime.total_milliseconds();
	}
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_ASSOCIATION_PARAMS_H

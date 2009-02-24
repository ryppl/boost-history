//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_INITMSG_PARAMS_H
#define BOOST_NET_IPV4_SCTP_INITMSG_PARAMS_H

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
{ class options; }

class initmsg_params
{
private:
	friend class detail::options;

	sctp_initmsg_type		si_;

	initmsg_params( sctp_initmsg_type si)
	: si_( si)
	{}

	operator sctp_initmsg_type const () const
	{ return si_; }

public:
	initmsg_params()
	: si_()
	{ ::bzero( & si_, sizeof( sctp_initmsg_type) ); }

	std::size_t outbound_streams() const
	{ return si_.sinit_num_ostreams; }

	void outbound_streams( std::size_t value)
	{
		BOOST_ASSERT( value >= 0);

		si_.sinit_num_ostreams = value;
	}

	std::size_t max_inbound_streams() const
	{ return si_.sinit_max_instreams; }

	void max_inbound_streams( std::size_t value)
	{
		BOOST_ASSERT( value >= 0);

		si_.sinit_max_instreams = value;
	}

	std::size_t max_attempts() const
	{ return si_.sinit_max_attempts; }

	void max_attempts( std::size_t value)
	{
		BOOST_ASSERT( value >= 0);

		si_.sinit_max_attempts = value;
	}

	const posix_time::time_duration max_init_timeout() const
	{ return posix_time::milliseconds( si_.sinit_max_init_timeo); }

	void max_init_timeout( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		si_.sinit_max_init_timeo = timeout.total_milliseconds();
	}
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_INITMSG_PARAMS_H

//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_RTO_PARAMS_H
#define BOOST_NET_IPV4_SCTP_RTO_PARAMS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <strings.h>
}

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

class rto_params
{
private:
	friend class detail::options;
	friend class detail::association_options;

	sctp_rtoinfo_type		sr_;

	rto_params( sctp_rtoinfo_type sr)
	: sr_( sr)
	{}

	operator sctp_rtoinfo_type const () const
	{ return sr_; }

public:
	rto_params()
	: sr_()
	{ ::bzero( & sr_, sizeof( sctp_rtoinfo_type) ); }

	const posix_time::time_duration initial_rto() const
	{ return posix_time::milliseconds( sr_.srto_initial); }

	void initial_rto( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		sr_.srto_initial = timeout.total_milliseconds();
	}

	const posix_time::time_duration maximum_rto() const
	{ return posix_time::milliseconds( sr_.srto_max); }

	void maximum_rto( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		sr_.srto_max = timeout.total_milliseconds();
	}

	const posix_time::time_duration minimum_rto() const
	{ return posix_time::milliseconds( sr_.srto_min); }

	void minimum_rto( posix_time::time_duration const& timeout)
	{
		BOOST_ASSERT( ! timeout.is_special() );
		BOOST_ASSERT( ! timeout.is_negative() );

		sr_.srto_min = timeout.total_milliseconds();
	}
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_RTO_PARAMS_H

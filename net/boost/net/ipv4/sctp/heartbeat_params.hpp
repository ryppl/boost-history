//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_HEARTBEAT_PARAMS_H
#define BOOST_NET_IPV4_SCTP_HEARTBEAT_PARAMS_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#include <netinet/sctp.h>
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

class heartbeat_params
{
private:
	friend class detail::options;
	friend class detail::association_options;

	sctp_paddrparams_type		sp_;

	heartbeat_params( sctp_paddrparams_type sp)
	: sp_( sp)
	{}

	operator sctp_paddrparams_type const () const
	{ return sp_; }

public:
	const posix_time::time_duration interval() const
	{ return posix_time::milliseconds( sp_.spp_hbinterval); }

	void interval( posix_time::time_duration const& interv)
	{
		BOOST_ASSERT( ! interv.is_special() );
		BOOST_ASSERT( ! interv.is_negative() );

		sp_.spp_hbinterval = interv.total_milliseconds();
	}

	bool is_enabled() const
	{ return sp_.spp_flags & SPP_HB_ENABLE; }

	void enable()
	{
		sp_.spp_flags |= SPP_HB_ENABLE;
		sp_.spp_flags &= ~SPP_HB_DISABLE;
	}

	bool is_disabled() const
	{ return sp_.spp_flags & SPP_HB_DISABLE; }

	void disable()
	{
		sp_.spp_flags |= SPP_HB_DISABLE;
		sp_.spp_flags &= ~SPP_HB_ENABLE;
	}

	bool is_immediately() const
	{ return sp_.spp_flags & SPP_HB_DEMAND; }

	void immediately()
	{ sp_.spp_flags |= SPP_HB_DEMAND; }

	bool is_zero() const
	{ return sp_.spp_flags & SPP_HB_TIME_IS_ZERO; }

	void zero()
	{ sp_.spp_flags |= SPP_HB_TIME_IS_ZERO; }

	std::size_t beats_before_inactive() const
	{ return sp_.spp_pathmaxrxt; }

	void beats_before_inactive( std::size_t value)
	{ sp_.spp_pathmaxrxt = value; }
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_HEARTBEAT_PARAMS_H

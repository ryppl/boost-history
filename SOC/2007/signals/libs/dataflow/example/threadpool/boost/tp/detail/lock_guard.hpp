//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_LOCK_GUARD_H
#define BOOST_TP_DETAIL_LOCK_GUARD_H

#include <boost/utility.hpp>

#include <boost/tp/detail/lock.hpp>

namespace boost { namespace tp {
namespace detail
{
	template< typename M0, typename M1 >
	class lockguard
	: private boost::noncopyable
	{
	private:
		M0	&	m0_;
		M1	&	m1_;
	
	public:
		lockguard(
			M0 & m0,
			M1 & m1)
		: m0_( m0), m1_( m1)
		{ lock( m0_, m1_); }
	
		~lockguard()
		{
			m0_.unlock();
			m1_.unlock();
		}
	};
}
} }

#endif // BOOST_TP_DETAIL_LOCK_GUARD_H

// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_CLN_POLICY_HPP_
#define NUMERIC_ADAPTOR_CLN_POLICY_HPP_


#include <boost/numeric_adaptor/ieee_policy.hpp>
#include <cln/cln.h>
#include <cln/float.h>


namespace impl {


template <typename ClnType>
struct cln_policy: ieee_policy<ClnType>
{
	static inline void sqrt(ClnType& r, const ClnType& a)
	{
		r = cln::sqrt(a);
	}

template <typename CT>
	static inline CT big_numeric_cast(const ClnType& b)
	{
		return cln::double_approx(b);
	}
};


} // namespace impl


typedef impl::cln_policy<cln::cl_FF> cln_ff_policy;
typedef impl::cln_policy<cln::cl_DF> cln_df_policy;


#endif

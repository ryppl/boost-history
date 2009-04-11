// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_GMP_POLICY_HPP_
#define NUMERIC_ADAPTOR_GMP_POLICY_HPP_


#include "gmp.h"


struct gmp_policy
{
	typedef mpf_t type;

	static inline void init(type& value)
	{
		mpf_init(value);
	}

	static inline void destruct(type& value)
	{
		mpf_clear(value);
	}

	template <typename CT>
	static inline void set(type& value, const CT& v)
	{
		mpf_set_d(value, v);
	}

	static inline void copy(const type& source, type& dest)
	{
		mpf_set(dest, source);
	}

	static inline void add(type& r, const type& a, const type& b)
	{
		mpf_add(r, a, b);
	}

	static inline void subtract(type& r, const type& a, const type& b)
	{
		mpf_sub(r, a, b);
	}

	static inline void multiply(type& r, const type& a, const type& b)
	{
		mpf_mul(r, a, b);
	}

	static inline void divide(type& r, const type& a, const type& b)
	{
		mpf_div(r, a, b);
	}

	static inline void sqrt(type& r, const type& a)
	{
		mpf_sqrt(r, a);
	}

	template <typename CT>
	static inline CT big_numeric_cast(const type& b)
	{
		return mpf_get_d(b);
	}

	static inline int compare(const type& a, const type& b)
	{
		return mpf_cmp(a, b);
	}
};


#endif

// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_FACULTY_HPP
#define BOOST_METAMATH_DOUBLE_FACULTY_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/constants.hpp>

namespace boost { namespace metamath {


template<int N>
struct faculty_impl_c {
	typedef typename mpl::times<mpl::int_<N>,typename faculty_impl_c<N-1>::type >::type type;
};

template<>
struct faculty_impl_c<0> {
	typedef meta::one type;
};

template<typename Tag>
struct faculty_impl;

template<>
struct faculty_impl<mpl::integral_c_tag>
{
	template<typename N> struct apply : faculty_impl_c<N::value>::type
	{
	};
};

}}

#endif

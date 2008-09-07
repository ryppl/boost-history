//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_INFO_H
#define BOOST_TP_INFO_H

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/tp/detail/info.hpp>

namespace boost { namespace tp
{
template< typename Pool >
struct has_priority
:
public boost::mpl::bool_<
	boost::is_same<
		detail::has_priority,
		typename Pool::queue_type::priority_tag_type
	>::value
>
{};

template< typename Pool >
struct priority_type
{
	typedef typename Pool::queue_type::attribute_type	type;
};

} }

#endif // BOOST_TP_INFO_H


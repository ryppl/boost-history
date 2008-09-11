/**
 * \file boost/mirror/detail/unnamed_attribs.hpp
 * Basic class for getting names of attributes of tuple-like classes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_UNNAMED_ATTRIBS_HPP
#define BOOST_MIRROR_DETAIL_UNNAMED_ATTRIBS_HPP

#include <boost/mirror/detail/static_int_to_str.hpp>

namespace boost {
namespace mirror {
namespace detail {
	//
	// specializations of this meta-function return a vector
	// containing the proper 'static string' that is used as
	// prefix for tuple member attribute names
	template <typename CharT>
	struct meta_class_tuple_attrib_name_prefix;

	template <>
	struct meta_class_tuple_attrib_name_prefix<char>
	{
		typedef mpl::vector_c<char, '_' > type;
	};
	template <>
	struct meta_class_tuple_attrib_name_prefix<wchar_t>
	{
		typedef mpl::vector_c<wchar_t, L'_' > type;
	};
} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


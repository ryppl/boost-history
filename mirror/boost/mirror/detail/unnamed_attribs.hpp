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

#define BOOST_MIRROR_DEFINE_MC_TUPLE_ATTRIB_NAME_PREFIX(R, DATA, I, CHAR_T)\
	template <> \
	struct meta_class_tuple_attrib_name_prefix< CHAR_T > \
	{ \
		typedef mpl::vector_c< \
			CHAR_T, \
			BOOST_CTS_LIT_CHAR_T(CHAR_T, '_') \
		> type; \
	};

BOOST_CTS_FOR_EACH_CHAR_T(
	BOOST_MIRROR_DEFINE_MC_TUPLE_ATTRIB_NAME_PREFIX,
	_
)

#undef BOOST_MIRROR_DEFINE_MC_TUPLE_ATTRIB_NAME_PREFIX

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


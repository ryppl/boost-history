/**
 * \file boost/mirror/intrinsic/detail/base_class_at.hpp
 * Implementation of the at algorithm for base_classes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_DETAIL_BASE_CLASS_AT_HPP
#define BOOST_MIRROR_INTRINSIC_DETAIL_BASE_CLASS_AT_HPP

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// meta_class_attributes
// meta_class_all_attributes
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {

namespace detail {

/** Implementation of the at function on meta_base_classes
 */
template <class MetaBaseClasses, class Position>
struct meta_inheritance_at
{
	typedef meta_inheritance<
		typename mpl::at<
			typename MetaBaseClasses::list,
			Position
		>::type 
	> type;
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


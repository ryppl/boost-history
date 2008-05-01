/**
 * \file boost/mirror/algorithm/detail/attribute_at.hpp
 * Implementation of the at algorithm
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_ATTRIBUTE_AT_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_ATTRIBUTE_AT_HPP

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// meta_class_attributes
// meta_class_all_attributes
#include <boost/mirror/meta_attributes.hpp>

namespace boost {
namespace mirror {

namespace detail {

/** Implementation of the for_each function on meta_attributes
 */
template <class reflected_class, class variant_tag, class meta_attributes, class position>
struct meta_attribute_at
{
	typedef meta_class_attribute<
		reflected_class, variant_tag,
		meta_attributes,
		position
	> type;
};

} // namespace detail

} // namespace mirror
} // namespace boost

#endif //include guard


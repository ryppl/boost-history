/**
 * \file boost/mirror/meta_classes/std_pair.hpp
 * Meta-class for std::pair<T1, T2>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_CLASSES_STD_PAIR_HPP
#define BOOST_MIRROR_META_CLASSES_STD_PAIR_HPP

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_types/std_pair.hpp>

namespace boost {
namespace mirror {

BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_BEGIN(::std::pair, 2)
	BOOST_MIRROR_REG_SIMPLE_TEMPLATE_ATTRIB(_, T0, first)
	BOOST_MIRROR_REG_SIMPLE_TEMPLATE_ATTRIB(_, T1, second)
BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_END


} // namespace mirror
} // namespace boost

#endif //include guard


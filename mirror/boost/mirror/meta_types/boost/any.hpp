/**
 * \file boost/mirror/meta_types/boost/any.hpp
 *
 * Registering of ::boost::any
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_ANY_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_ANY_HPP

// register string types
#include <boost/mirror/meta_type.hpp>
#include <boost/any.hpp>

namespace boost {
namespace mirror {

/** Register the ::boost::any type
 */
BOOST_MIRROR_REG_TYPE(::boost, any)


} // namespace mirror
} // namespace boost

#endif //include guard


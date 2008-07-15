/**
 * \file boost/mirror/meta_types/_native.hpp
 *
 * Registering of native C++ types
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_NATIVE_HPP
#define BOOST_MIRROR_META_TYPES_NATIVE_HPP

// meta namespaces
#include <boost/mirror/meta_namespace.hpp>
// registering of meta-types
#include <boost/mirror/detail/meta_type_registering.hpp>

namespace boost {
namespace mirror {


/** Register C++ native types
 */
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(void)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(bool)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(char)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(unsigned char)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(signed char)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(wchar_t)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(short int)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(unsigned short int)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(int)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(unsigned int)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(long int)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(unsigned long int)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(float)
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(double)

BOOST_MIRROR_REG_TYPEDEF(::std, size_t)

} // namespace mirror
} // namespace boost

#endif //include guard


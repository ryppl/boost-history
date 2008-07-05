/**
 * \file boost/mirror/meta_types/std_string.hpp
 *
 * Registering of native C++ string types
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_STRING_HPP
#define BOOST_MIRROR_META_TYPES_STD_STRING_HPP

// register native types
#include <boost/mirror/meta_types/_native.hpp>

namespace boost {
namespace mirror {

/** Register std string and wstring
 */
BOOST_MIRROR_REG_TYPE(::std, string)
BOOST_MIRROR_REG_TYPE(::std, wstring)

} // namespace mirror
} // namespace boost

#endif //include guard


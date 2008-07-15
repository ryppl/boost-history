/**
 * \file boost/mirror/meta_types/std_string.hpp
 *
 * Registering of native C++ string types
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_BSTRING_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_BSTRING_HPP

// register string types
#include <boost/mirror/meta_types/std_string.hpp>
#include <boost/char_type_switch/string.hpp>
#include <boost/mirror/meta_namespaces/boost_cts.hpp>

namespace boost {
namespace mirror {

/** Register the bchar and bstring 
 */
BOOST_MIRROR_REG_TYPEDEF(::boost::cts, bchar)
BOOST_MIRROR_REG_TYPEDEF(::boost::cts, bstring)


} // namespace mirror
} // namespace boost

#endif //include guard


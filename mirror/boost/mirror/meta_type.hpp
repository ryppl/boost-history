/**
 * \file boost/mirror/meta_type.hpp
 * Registering and reflection of types
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPE_HPP
#define BOOST_MIRROR_META_TYPE_HPP

//
// registering of meta-types
#include <boost/mirror/detail/meta_type_registering.hpp>
// register native types
#include <boost/mirror/meta_types/_native.hpp>
#include <boost/mirror/meta_types/std/string.hpp>
// register bstrings
#include <boost/mirror/meta_types/boost/cts/bstring.hpp>
//
// non-trivial types
#include <boost/mirror/meta_types/_const_volatile.hpp>
#include <boost/mirror/meta_types/_ptr_ref.hpp>
#include <boost/mirror/meta_types/_array.hpp>
#include <boost/mirror/meta_types/_free_fn.hpp>

namespace boost {
namespace mirror {


} // namespace mirror
} // namespace boost

#endif //include guard


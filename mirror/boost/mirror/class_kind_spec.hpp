/**
 * \file boost/mirror/class_kind_spec.hpp
 * Definition of token types representing class kind specifiers
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CLASS_KIND_SPEC
#define BOOST_MIRROR_CLASS_KIND_SPEC

// access specifiers
#include <boost/mirror/access_spec.hpp>


namespace boost {
namespace mirror {

/** Class 'kind' (class|struct|union) specifier
 */
struct class_kind_specifier { };

struct class_ : class_kind_specifier { };
struct struct_ : class_kind_specifier { };
struct union_ : class_kind_specifier { };

template <class C>
struct meta_class_kind
{
	typedef class_ result;
};

/** Default inheritance access specifier for the given class kind
 */
template <class K>
struct class_kind_default_access
{
	typedef public_ specifier;
};

template <>
struct class_kind_default_access<class_>
{
	typedef private_ specifier;
};

} // namespace mirror
} // namespace boost

#endif //include guard


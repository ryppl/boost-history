/**
 * \file boost/mirror/inherit_spec.hpp
 * Definition of token types representing inheritance type specifiers
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INHERIT_SPEC
#define BOOST_MIRROR_INHERIT_SPEC

namespace boost {
namespace mirror {

/** Virtual vs. non-virtual inheritance specifier
 */
struct inheritance_specifier { };
struct virtual_base_ : inheritance_specifier { };
struct nonvirtual_base_ : inheritance_specifier { };


} // namespace mirror
} // namespace boost

#endif //include guard


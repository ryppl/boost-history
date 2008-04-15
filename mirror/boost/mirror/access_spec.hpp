/**
 * \file boost/mirror/access_spec.hpp
 * Definition of token types representing member access specifiers
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ACCESS_SPEC
#define BOOST_MIRROR_ACCESS_SPEC

namespace boost {
namespace mirror {

/** Access specifier 'keywords' 
 */
struct access_specifier { };

struct private_ : access_specifier { };
struct protected_ : access_specifier { };
struct public_ : access_specifier { };

} // namespace mirror
} // namespace boost

#endif //include guard


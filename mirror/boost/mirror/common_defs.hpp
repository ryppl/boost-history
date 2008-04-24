/**
 * \file boost/mirror/common_defs.hpp
 * Common definitions for the mirror library
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_COMMON_DEFS_HPP
#define BOOST_MIRROR_COMMON_DEFS_HPP

namespace boost {
namespace mirror {

/** Defines a constant class member attribute, named NAME
 *  initialized with the VALUE and possibly being instace of TYPE
 */
#define BOOST_MIRROR_CONST_MEMBER_ATTRIB(TYPE, NAME, VALUE) \
	enum {NAME = VALUE};

} // namespace mirror
} // namespace boost

#endif //include guard


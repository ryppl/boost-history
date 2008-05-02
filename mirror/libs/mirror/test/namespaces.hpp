/**
 * \file test/namespaces.hpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_MIRROR_LIBS_MIRROR_TEST_NAMESPACES_HPP
#define BOOST_MIRROR_LIBS_MIRROR_TEST_NAMESPACES_HPP

// namespace registering 
#include <boost/mirror/meta_namespace.hpp>

namespace test {
namespace feature {
namespace detail {

} // namespace detail
} // namespace feature

namespace stuff {
namespace detail {

} // namespace detail
} // namespace stuff
} // namespace test


namespace boost { 
namespace mirror {

// register the namespaces
//
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(test)
//
BOOST_MIRROR_REG_META_NAMESPACE(_test, feature)
BOOST_MIRROR_REG_META_NAMESPACE(_test_feature, detail)
// 
BOOST_MIRROR_REG_META_NAMESPACE(_test, stuff)
BOOST_MIRROR_REG_META_NAMESPACE(_test_stuff, detail)

} // namespace mirror
} // namespace boost

#endif // include guard


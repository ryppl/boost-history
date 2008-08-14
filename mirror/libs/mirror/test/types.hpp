/**
 * \file test/types.hpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing type registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_MIRROR_LIBS_MIRROR_TEST_TYPES_HPP
#define BOOST_MIRROR_LIBS_MIRROR_TEST_TYPES_HPP

#include "./namespaces.hpp"
#include <boost/mirror/meta_type.hpp>

namespace test {
namespace feature {
namespace detail {

struct foo_impl;

} // namespace detail

struct foo;

} // namespace feature

struct bar;

typedef feature::foo foobar;

} // namespace test

// type on the global scope
struct baz;

namespace boost { 
namespace mirror {

//
// register the nested types 
BOOST_MIRROR_REG_TYPE(::test::feature::detail, foo_impl)
BOOST_MIRROR_REG_TYPE(::test::feature, foo)
BOOST_MIRROR_REG_TYPE(::test, bar)
// register the nested typedef
BOOST_MIRROR_REG_TYPEDEF(::test, foobar)
// register the type defined on the global scope
BOOST_MIRROR_REG_TYPE_GS(baz)

} // namespace mirror
} // namespace boost

#endif // include guard


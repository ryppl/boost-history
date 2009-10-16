// Generic Geometry Library test file
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_TEST_TEST_COMMON_TEST_POINT_HPP
#define GGL_TEST_TEST_COMMON_TEST_POINT_HPP

#include <ggl/core/access.hpp>
#include <ggl/core/coordinate_type.hpp>
#include <ggl/core/coordinate_system.hpp>
#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/cs.hpp>
#include <ggl/core/tag.hpp>


namespace test
{

// Test point class

struct test_point
{
    float c1, c2, c3;
};


// Struct set of metafunctions to read/write coordinates above, specialized per dimension
template<int I>
struct accessor {};

template<>
struct accessor<0>
{
    static inline const float& get(const test_point& p) { return p.c1; }
    static inline void set(test_point& p, const float& value) { p.c1 = value; }
};

template<>
struct accessor<1>
{
    static inline const float& get(const test_point& p) { return p.c2; }
    static inline void set(test_point& p, const float& value) { p.c2 = value; }
};

template<>
struct accessor<2>
{
    static inline const float& get(const test_point& p) { return p.c3; }
    static inline void set(test_point& p, const float& value) { p.c3 = value; }
};


} // namespace test


namespace ggl { namespace traits {

template<>
struct tag<test::test_point> { typedef point_tag type; };

template<>
struct coordinate_type<test::test_point> { typedef float type; };

template<>
struct coordinate_system<test::test_point> { typedef cs::cartesian type; };

template<>
struct dimension<test::test_point>: boost::mpl::int_<3> {};

template<> struct access<test::test_point>
{
    template <int I>
    static inline const float& get(const test::test_point& p)
    {
        return test::accessor<I>::get(p);
    }

    template <int I>
    static inline void set(test::test_point& p, const float& value)
    {
        test::accessor<I>::set(p, value);
    }
};

}} // namespace ggl::traits

#endif // GGL_TEST_TEST_COMMON_TEST_POINT_HPP

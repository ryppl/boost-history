// Generic Geometry Library test file
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_TEST_COMMON_WITH_POINTER_HPP
#define GGL_TEST_COMMON_WITH_POINTER_HPP


#include <ggl/core/access.hpp>
#include <ggl/core/coordinate_type.hpp>
#include <ggl/core/coordinate_system.hpp>
#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/cs.hpp>
#include <ggl/core/tag.hpp>


namespace test
{

// Sample point, having x/y
struct test_point_xy
{
    float x,y;
};

}


namespace ggl { namespace traits {

template<> struct tag<test::test_point_xy*>
{ typedef point_tag type; };

template<> struct coordinate_type<test::test_point_xy*>
{ typedef double type; };

template<> struct coordinate_system<test::test_point_xy*>
{ typedef cs::cartesian type; };

template<> struct dimension<test::test_point_xy*> : boost::mpl::int_<2> {};

template<>
struct access<test::test_point_xy*>
{
    template <std::size_t I>
    static double get(const test::test_point_xy* p)
    {
        return I == 0 ? p->x : p->y;
        // Or implement an accessor with specializations
    }

    template <int I>
    static void set(test::test_point_xy* p, const double& value)
    {
        // Or (better) implement an accessor with specializations
        if (I == 0) p->x = value;
        else if (I == 1) p->y = value;
    }

};

}} // namespace ggl::traits


#endif // #ifndef GGL_TEST_COMMON_WITH_POINTER_HPP

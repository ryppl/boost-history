// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_CONCEPTS_DETAIL_CHECK_APPEND_HPP
#define GGL_GEOMETRIES_CONCEPTS_DETAIL_CHECK_APPEND_HPP


#include <boost/type_traits/remove_const.hpp>

#include <ggl/core/access.hpp>



namespace ggl { namespace concept {

namespace detail
{

    template <typename Geometry, typename Point, bool UseStd>
    struct check_append
    {};

    template <typename Geometry, typename Point>
    struct check_append<Geometry, Point, true>
    {
        static void apply(Geometry& geometry, Point const& p)
        {
            geometry.push_back(p);
        }
    };

    template <typename Geometry, typename Point>
    struct check_append<Geometry, Point, false>
    {
        static void apply(Geometry& geometry, Point const& p)
        {
            ggl::traits::append_point
                <
                     typename boost::remove_const<Geometry>::type,
                     Point
                >::apply(geometry, p, -1, -1);
        }
    };
}



}} // namespace ggl::concept


#endif // GGL_GEOMETRIES_CONCEPTS_DETAIL_CHECK_APPEND_HPP

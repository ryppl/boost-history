// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_CONCEPTS_DETAIL_CHECK_CLEAR_HPP
#define GGL_GEOMETRIES_CONCEPTS_DETAIL_CHECK_CLEAR_HPP


#include <boost/type_traits/remove_const.hpp>

#include <ggl/core/access.hpp>


namespace ggl { namespace concept {


#ifndef DOXYGEN_NO_DETAIL
namespace detail
{

    template <typename Geometry, bool UseStd>
    struct check_clear
    {};

    template <typename Geometry>
    struct check_clear<Geometry, true>
    {
        static void apply(Geometry& geometry)
        {
            geometry.clear();
        }
    };


    template <typename Geometry>
    struct check_clear<Geometry, false>
    {
        static void apply(Geometry& geometry)
        {
            ggl::traits::clear
                <
                     typename boost::remove_const<Geometry>::type
                >::apply(geometry);
        }
    };

}
#endif // DOXYGEN_NO_DETAIL


}} // namespace ggl::concept


#endif // GGL_GEOMETRIES_CONCEPTS_DETAIL_CHECK_CLEAR_HPP

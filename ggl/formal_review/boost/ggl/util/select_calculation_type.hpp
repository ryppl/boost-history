// Generic Geometry Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_UTIL_SELECT_CALCULATION_TYPE_HPP
#define GGL_UTIL_SELECT_CALCULATION_TYPE_HPP


#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

#include <ggl/util/select_coordinate_type.hpp>


namespace ggl
{


/*!
    \brief Meta-function selecting the "calculation" type
    \details Based on two input point types, and an input calculation type,
        (which defaults to void in the calling function), this meta-function
        selects the most appropriate:
        - if calculation type is specified, that one is used,
        - if it is void, the most precise of the two points is used
    \ingroup utility
 */
template <typename Point1, typename Point2, typename CalculationType>
struct select_calculation_type
{
    typedef typename
        boost::mpl::if_c
        <
            boost::is_void<CalculationType>::type::value,
            typename select_coordinate_type
                <
                    Point1,
                    Point2
                >::type,
            CalculationType
        >::type type;
};


} // namespace ggl


#endif // GGL_UTIL_SELECT_CALCULATION_TYPE_HPP

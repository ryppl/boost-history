// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_UTIL_AS_RANGE_HPP
#define GGL_MULTI_UTIL_AS_RANGE_HPP


#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <ggl/multi/core/is_multi.hpp>

#include <ggl/util/as_range.hpp>

namespace ggl {

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename Tag, typename Geometry>
struct as_range_type<Tag, true, Geometry>
{
    typedef typename ggl::as_range_type
        <
            typename boost::range_value<Geometry>::type
        > type;
};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


} // namespace ggl

#endif // GGL_MULTI_UTIL_AS_RANGE_HPP

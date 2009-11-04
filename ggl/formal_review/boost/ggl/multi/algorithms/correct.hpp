// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_ALGORITHMS_CORRECT_HPP
#define GGL_MULTI_ALGORITHMS_CORRECT_HPP

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>


#include <ggl/algorithms/correct.hpp>

#include <ggl/multi/core/tags.hpp>



namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace correct {

template <typename MultiPolygon>
struct correct_multi_polygon
{
    static inline void apply(MultiPolygon& mp)
    {
        typedef typename boost::range_value<MultiPolygon>::type polygon_type;
        for (typename boost::range_iterator<MultiPolygon>::type it 
                    = boost::begin(mp); 
            it != boost::end(mp); 
            ++it)
        {
            correct_polygon<polygon_type>::apply(*it);
        }
    }
};

}} // namespace detail::correct
#endif

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename Geometry>
struct correct<multi_polygon_tag, Geometry>
    : detail::correct::correct_multi_polygon<Geometry>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


} // namespace ggl


#endif // GGL_MULTI_ALGORITHMS_CORRECT_HPP

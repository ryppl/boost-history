// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_MULTI_STRATEGIES_CARTESIAN_CENTROID_AVERAGE_HPP
#define GGL_MULTI_STRATEGIES_CARTESIAN_CENTROID_AVERAGE_HPP


#include <boost/numeric/conversion/cast.hpp>
#include <boost/type_traits.hpp>

#include <ggl/core/coordinate_type.hpp>
#include <ggl/core/point_type.hpp>
#include <ggl/arithmetic/arithmetic.hpp>
#include <ggl/strategies/centroid.hpp>
#include <ggl/util/copy.hpp>


namespace ggl
{

namespace strategy { namespace centroid_ {


template
<
    typename PointCentroid,
    typename Point = PointCentroid
>
class centroid_average
{
private :

    /*! subclass to keep state */
    class sum
    {
        friend class centroid_average;
        int count;
        PointCentroid centroid;

    public :
        inline sum()
            : count(0)
        {
            assign_zero(centroid);
        }
    };

public :
    typedef sum state_type;
    typedef PointCentroid centroid_point_type;
    typedef Point point_type;

    static inline void apply(Point const& p, sum& state)
    {
        add_point(state.centroid, p);
        state.count++;
    }

    static inline void result(sum const& state, PointCentroid& centroid)
    {
        centroid = state.centroid;
        divide_value(centroid, state.count);
    }

};


}} // namespace strategy::centroid


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS



template <typename Point, typename Geometry>
struct strategy_centroid<cartesian_tag, multi_point_tag, 2, Point, Geometry>
{
    typedef strategy::centroid_::centroid_average
        <
            Point,
            typename point_type<Geometry>::type
        > type;
};

#endif


} // namespace ggl


#endif // GGL_MULTI_STRATEGIES_CARTESIAN_CENTROID_AVERAGE_HPP

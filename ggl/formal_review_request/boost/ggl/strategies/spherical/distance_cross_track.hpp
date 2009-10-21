// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGY_SPHERICAL_DISTANCE_CROSS_TRACK_HPP
#define GGL_STRATEGY_SPHERICAL_DISTANCE_CROSS_TRACK_HPP


#include <ggl/core/cs.hpp>
#include <ggl/core/concepts/point_concept.hpp>
#include <ggl/core/access.hpp>
#include <ggl/core/radian_access.hpp>


#include <ggl/strategies/strategy_traits.hpp>

#include <ggl/strategies/distance_result.hpp>

#include <ggl/util/math.hpp>
//#include <ggl/util/write_dsv.hpp>



namespace ggl
{
namespace strategy
{

    namespace distance
    {

        /*!
            \brief Strategy functor for distance point to segment calculation
            \ingroup distance
            \details Class which calculates the distance of a point to a segment, using latlong points
            \see http://williams.best.vwh.net/avform.htm
            \tparam P point type
            \tparam S segment type
        */
        template <typename P, typename S>
        class cross_track
        {
            public :
                typedef double return_type;
                typedef P point_type;
                typedef typename strategy_distance
                    <
                        typename ggl::cs_tag<P>::type, 
                        typename ggl::cs_tag<P>::type,
                        P, P
                    >::type point_strategy_type;


                inline cross_track(double r = 1.0) 
                    : m_radius(r)
                    , m_strategy(1.0) // Keep this 1.0 and not r
                {}


                // It might be useful in the future
                // to overload constructor with strategy info.


                inline return_type operator()(P const& p, S const& s) const
                {
                    // ASSUMPTION: segment
                    // However, this is normally called from internal functions
                    // Future: solve this using other functions using get<,>
                    return calc(p, s.first, s.second);
                }

            private :
                double m_radius;

                // Point-point distances are calculated in radians, on the unit sphere
                point_strategy_type m_strategy;

                /// Calculate course (bearing) between two points. Might be moved to a "course formula" ...
                inline double course(P const& p1, P const& p2) const
                {
                    // http://williams.best.vwh.net/avform.htm#Crs
                    double dlon = get_as_radian<0>(p2) - get_as_radian<0>(p1);
                    double cos_p2lat = cos(get_as_radian<1>(p2));

                    // "An alternative formula, not requiring the pre-computation of d"
                    return atan2(sin(dlon) * cos_p2lat,
                        cos(get_as_radian<1>(p1)) * sin(get_as_radian<1>(p2))
                        - sin(get_as_radian<1>(p1)) * cos_p2lat * cos(dlon));
                }

                inline return_type calc(P const& p, P const& sp1, P const& sp2) const
                {
                    // http://williams.best.vwh.net/avform.htm#XTE

                    double d1 = m_strategy(sp1, p);

                    // Actually, calculation of d2 not necessary if we know that the projected point is on the great circle...
                    double d2 = m_strategy(sp2, p);

                    double crs_AD = course(sp1, p);
                    double crs_AB = course(sp1, sp2);
                    double XTD = std::abs(asin(sin(d1) * sin(crs_AD - crs_AB)));

//std::cout << "Course " << dsv(sp1) << " to " << dsv(p) << " " << crs_AD * ggl::math::r2d << std::endl;
//std::cout << "Course " << dsv(sp1) << " to " << dsv(sp2) << " " << crs_AB * ggl::math::r2d << std::endl;
//std::cout << "XTD: " << (XTD * 6373.0) << " d1: " <<  (d1 * 6373.0)  << " d2: " <<  (d2 * 6373.0)  << std::endl;


                    // Return shortest distance, either to projected point on segment sp1-sp2, or to sp1, or to sp2
                    return return_type(m_radius * (std::min)((std::min)(d1, d2), XTD));
                }
        };

    } // namespace distance

} // namespace strategy


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS


template <typename Point, typename Segment>
struct strategy_distance_segment<spherical_tag, spherical_tag, Point, Segment>
{
    typedef strategy::distance::cross_track<Point, Segment> type;
};


// Use this point-segment for geographic as well. TODO: change this, extension!
template <typename Point, typename Segment>
struct strategy_distance_segment<geographic_tag, geographic_tag, Point, Segment>
{
    typedef strategy::distance::cross_track<Point, Segment> type;
};


template <typename Point, typename Segment>
struct strategy_tag<strategy::distance::cross_track<Point, Segment> >
{
    typedef strategy_tag_distance_point_segment type;
};



#endif






} // namespace ggl


#endif // GGL_STRATEGY_SPHERICAL_DISTANCE_CROSS_TRACK_HPP

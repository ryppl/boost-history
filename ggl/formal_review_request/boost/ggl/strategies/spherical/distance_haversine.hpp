// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGY_SPHERICAL_DISTANCE_HPP
#define GGL_STRATEGY_SPHERICAL_DISTANCE_HPP


#include <ggl/core/cs.hpp>
#include <ggl/core/access.hpp>
#include <ggl/core/radian_access.hpp>


#include <ggl/strategies/strategy_traits.hpp>

#include <ggl/strategies/distance_result.hpp>

#include <ggl/util/get_cs_as_radian.hpp>



namespace ggl
{
namespace strategy
{

    namespace distance
    {

        /*!
            \brief Distance calculation for spherical coordinates on a perfect sphere using haversine
            \ingroup distance
            \tparam P1 first point type
            \tparam P2 optional second point type
            \author Adapted from: http://williams.best.vwh.net/avform.htm
            \see http://en.wikipedia.org/wiki/Great-circle_distance
            \note It says: <em>The great circle distance d between two points with coordinates {lat1,lon1} and {lat2,lon2} is given by:
                        d=acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon1-lon2))
                    A mathematically equivalent formula, which is less subject to rounding error for short distances is:
                        d=2*asin(sqrt((sin((lat1-lat2)/2))^2 + cos(lat1)*cos(lat2)*(sin((lon1-lon2)/2))^2))</em>
        */
        template <typename P1, typename P2 = P1>
        class haversine
        {
            public :
                typedef double return_type;
                typedef P1 point_type;

                inline haversine(double r = 1.0)
                    : m_radius(r)
                {}

                inline return_type operator()(P1 const& p1, P2 const& p2) const
                {
                    return calc(get_as_radian<0>(p1), get_as_radian<1>(p1),
                                    get_as_radian<0>(p2), get_as_radian<1>(p2));
                }

            private :
                double m_radius;
                typedef typename coordinate_type<P1>::type T1;
                typedef typename coordinate_type<P2>::type T2;

                inline return_type calc(T1 const& lon1, T1 const& lat1, T2 const& lon2, T2 const& lat2) const
                {
                    double a = math::hav(lat2 - lat1) + cos(lat1) * cos(lat2) * math::hav(lon2 - lon1);
                    double c = 2.0 * asin(sqrt(a));
                    return return_type(m_radius * c);
                }
        };



    } // namespace distance




} // namespace strategy


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
template <typename P1, typename P2>
struct strategy_distance<spherical_tag, spherical_tag, P1, P2>
{
    typedef strategy::distance::haversine<P1, P2> type;
};






template <typename P1, typename P2>
struct strategy_tag<strategy::distance::haversine<P1, P2> >
{
    typedef strategy_tag_distance_point_point type;
};




#endif






} // namespace ggl


#endif // GGL_STRATEGY_SPHERICAL_DISTANCE_HPP

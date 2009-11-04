// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_EXTENSIONS_GIS_GEOGRAPHIC_STRATEGIES_AREA_HUILLER_EARTH_HPP
#define GGL_EXTENSIONS_GIS_GEOGRAPHIC_STRATEGIES_AREA_HUILLER_EARTH_HPP



#include <ggl/strategies/spherical/area_huiller.hpp>


namespace ggl
{

namespace strategy { namespace area {



template
<
    typename PointOfSegment,
    typename CalculationType = void
>
class huiller_earth
    : public huiller<PointOfSegment, CalculationType>
{
public :
    // By default the average earth radius.
    // Uses can specify another radius.
    // Note that the earth is still spherical
    inline huiller_earth(double radius = 6372795.0)
        : huiller<PointOfSegment, CalculationType>(radius)
    {}
};



}} // namespace strategy::area


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS

template <typename Point>
struct strategy_area<geographic_tag, Point>
{
    typedef strategy::area::huiller_earth<Point> type;
};

#endif


} // namespace ggl


#endif // GGL_EXTENSIONS_GIS_GEOGRAPHIC_STRATEGIES_AREA_HUILLER_EARTH_HPP

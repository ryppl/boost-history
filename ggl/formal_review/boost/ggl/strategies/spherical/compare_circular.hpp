// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_SPHERICAL_COMPARE_SPHERICAL_HPP
#define GGL_STRATEGIES_SPHERICAL_COMPARE_SPHERICAL_HPP


#include <ggl/core/cs.hpp>
#include <ggl/core/tags.hpp>


#include <ggl/strategies/compare.hpp>


namespace ggl
{

namespace strategy { namespace compare {


#ifndef DOXYGEN_NO_DETAIL
namespace detail {


template <typename Units>
struct shift
{
};


template <>
struct shift<degree>
{
    static inline double full() { return 360.0; }
    static inline double half() { return 180.0; }
};

template <>
struct shift<radian>
{
    static inline double full() { return ggl::math::two_pi; }
    static inline double half() { return ggl::math::pi; }
};

} // namespace detail
#endif

/*!
    \brief Compare (in one direction) strategy for spherical coordinates
    \ingroup util
    \tparam Point point-type
    \tparam Dimension dimension
*/
template <typename CoordinateType, typename Units, typename Compare>
struct circular_comparator
{
    static inline CoordinateType put_in_range(CoordinateType const& c,
            double min_border, double max_border)
    {
        CoordinateType value = c;
        while (value < min_border)
        {
            value += detail::shift<Units>::full();
        }
        while (value > max_border)
        {
            value -= detail::shift<Units>::full();
        }
        return value;
    }

    inline bool operator()(CoordinateType const& c1, CoordinateType const& c2)  const
    {
        Compare compare;

        // Check situation that one of them is e.g. std::numeric_limits.
        static const double full = detail::shift<Units>::full();
        double mx = 10.0 * full;
        if (c1 < -mx || c1 > mx || c2 < -mx || c2 > mx)
        {
            // do normal comparison, using circular is not useful
            return compare(c1, c2);
        }

        static const double half = full / 2.0;
        CoordinateType v1 = put_in_range(c1, -half, half);
        CoordinateType v2 = put_in_range(c2, -half, half);

        // Two coordinates on a circle are
        // at max <= half a circle away from each other.
        // So if it is more, shift origin.
        CoordinateType diff = std::abs(v1 - v2);
        if (diff > half)
        {
            v1 = put_in_range(v1, 0, full);
            v2 = put_in_range(v2, 0, full);
        }

        return compare(v1, v2);
    }
};


}} // namespace strategy::compare


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS

// Specialize for the longitude (dim 0)
template
<
    typename Point,
    template<typename> class CoordinateSystem,
    typename Units
>
struct strategy_compare<spherical_tag, 1, Point, CoordinateSystem<Units>, 0>
{
    typedef typename coordinate_type<Point>::type coordinate_type;
    typedef strategy::compare::circular_comparator
        <
            coordinate_type,
            Units,
            std::less<coordinate_type>
        > type;
};

template
<
    typename Point,
    template<typename> class CoordinateSystem,
    typename Units
>
struct strategy_compare<spherical_tag, -1, Point, CoordinateSystem<Units>, 0>
{
    typedef typename coordinate_type<Point>::type coordinate_type;
    typedef strategy::compare::circular_comparator
        <
            coordinate_type,
            Units,
            std::greater<coordinate_type>
        > type;
};



#endif


} // namespace ggl


#endif // GGL_STRATEGIES_SPHERICAL_COMPARE_SPHERICAL_HPP
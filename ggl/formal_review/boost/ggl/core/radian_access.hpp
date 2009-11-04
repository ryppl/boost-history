// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_CORE_RADIAN_ACCESS_HPP
#define GGL_CORE_RADIAN_ACCESS_HPP


#include <cstddef>

#include <boost/numeric/conversion/cast.hpp>

#include <ggl/core/access.hpp>
#include <ggl/core/cs.hpp>


#include <ggl/util/math.hpp>


namespace ggl {


#ifndef DOXYGEN_NO_DETAIL
namespace detail {

template <std::size_t Dimension, typename Geometry, typename DegreeOrRadian>
struct radian_access
{
};

template
<
    std::size_t Dimension,
    typename Geometry,
    template<typename> class CoordinateSystem
>
struct radian_access<Dimension, Geometry, CoordinateSystem<radian> >
{
    typedef typename coordinate_type<Geometry>::type coordinate_type;

    static inline coordinate_type get(Geometry const& geometry)
    {
        return ggl::get<Dimension>(geometry);
    }

    static inline void set(Geometry& geometry, coordinate_type const& radians)
    {
        ggl::set<Dimension>(geometry, radians);
    }
};

template
<
    std::size_t Dimension,
    typename Geometry,
    template<typename> class CoordinateSystem
>
struct radian_access<Dimension, Geometry, CoordinateSystem<degree> >
{
    typedef typename coordinate_type<Geometry>::type coordinate_type;

    static inline coordinate_type get(Geometry const& geometry)
    {
        return boost::numeric_cast
            <
                coordinate_type
            >(ggl::get<Dimension>(geometry) * ggl::math::d2r);
    }

    static inline void set(Geometry& geometry, coordinate_type const& radians)
    {
        ggl::set<Dimension>(geometry, boost::numeric_cast
            <
                coordinate_type
            >(radians * ggl::math::r2d));
    }

};

} // namespace detail
#endif // DOXYGEN_NO_DETAIL


/*!
    \brief get coordinate value of a point, result is in Radian
    \details Result is in Radian, even if source coordinate system
        is in Degrees
    \return coordinate value
    \ingroup access
    \tparam Dimension dimension
    \tparam Geometry geometry
    \param geometry geometry to get coordinate value from
    \note Only applicable to coordinate systems templatized by units,
        e.g. spherical or geographic coordinate systems
*/
template <std::size_t Dimension, typename Geometry>
inline typename coordinate_type<Geometry>::type get_as_radian(const Geometry& geometry)
{
    return detail::radian_access<Dimension, Geometry,
            typename coordinate_system<Geometry>::type>::get(geometry);
}


/*!
    \brief set coordinate value (in radian) to a point
    \details Coordinate value will be set correctly, if coordinate system of
        point is in Degree, Radian value will be converted to Degree
    \ingroup access
    \tparam Dimension dimension
    \tparam Geometry geometry
    \param geometry geometry to assign coordinate to
    \param radians coordinate value to assign
    \note Only applicable to coordinate systems templatized by units,
        e.g. spherical or geographic coordinate systems
*/
template <std::size_t Dimension, typename Geometry>
inline void set_from_radian(Geometry& geometry,
            const typename coordinate_type<Geometry>::type& radians)
{
    detail::radian_access<Dimension, Geometry,
            typename coordinate_system<Geometry>::type>::set(geometry, radians);
}


} // namespace ggl


#endif // GGL_CORE_RADIAN_ACCESS_HPP

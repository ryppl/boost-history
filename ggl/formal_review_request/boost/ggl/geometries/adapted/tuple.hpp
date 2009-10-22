// Generic Geometry Library
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_GEOMETRIES_ADAPTED_TUPLE_HPP
#define GGL_GEOMETRIES_ADAPTED_TUPLE_HPP

#include <boost/tuple/tuple.hpp>

#include <ggl/core/coordinate_dimension.hpp>
#include <ggl/core/coordinate_type.hpp>
#include <ggl/core/point_type.hpp>
#include <ggl/core/tags.hpp>

namespace ggl
{

#ifndef DOXYGEN_NO_TRAITS_SPECIALIZATIONS
namespace traits
{

// boost::tuple, 2D
template <typename CoordinateType>
struct coordinate_type<boost::tuple<CoordinateType, CoordinateType> >
{
    typedef CoordinateType type;
};

template <typename CoordinateType>
struct dimension<boost::tuple<CoordinateType, CoordinateType> > 
    : boost::mpl::int_<2> 
{};

template <typename CoordinateType, std::size_t Dimension>
struct access
    <
        boost::tuple<CoordinateType, CoordinateType>,
        Dimension
    >
{
    static inline CoordinateType get(
        boost::tuple<CoordinateType, CoordinateType> const& point)
    {
        return point.template get<Dimension>();
    }

    static inline void set(boost::tuple<CoordinateType, CoordinateType>& point, 
        CoordinateType const& value)
    {
        point.template get<Dimension>() = value;
    }
};

template <typename CoordinateType>
struct tag<boost::tuple<CoordinateType, CoordinateType> >
{
    typedef point_tag type;
};

// boost::tuple, 3D
template <typename CoordinateType>
struct coordinate_type
    <
        boost::tuple<CoordinateType, CoordinateType, CoordinateType> 
    >
{
    typedef CoordinateType type;
};

template <typename CoordinateType>
struct dimension<boost::tuple<CoordinateType, CoordinateType, CoordinateType> > 
    : boost::mpl::int_<3> 
{};

template <typename CoordinateType, std::size_t Dimension>
struct access
    <
        boost::tuple<CoordinateType, CoordinateType, CoordinateType>,
        Dimension 
    >
{
    static inline CoordinateType get(
        boost::tuple
            <
                CoordinateType, CoordinateType, CoordinateType
            > const& point)
    {
        return point.template get<Dimension>();
    }

    static inline void set(
        boost::tuple<CoordinateType, CoordinateType, CoordinateType>& point, 
        CoordinateType const& value)
    {
        point.template get<Dimension>() = value;
    }
};

template <typename CoordinateType>
struct tag<boost::tuple<CoordinateType, CoordinateType, CoordinateType> >
{
    typedef point_tag type;
};

// The library user has
// 1) either to specify the coordinate system using a traits class
// 2) or include <ggl/geometries/adapted/tuple_@.hpp> 
//          where @=cartesian,geographic,...

} // namespace traits
#endif // DOXYGEN_NO_TRAITS_SPECIALIZATIONS

} // namespace ggl

#endif // GGL_GEOMETRIES_ADAPTED_TUPLE_HPP


// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_ADAPTED_STD_PAIR_AS_SEGMENT_HPP
#define GGL_GEOMETRIES_ADAPTED_STD_PAIR_AS_SEGMENT_HPP

// Only possible if the std::pair is not used for iterator/pair
// (maybe it is possible to avoid that by detecting in the other file
//  if an iterator was used in the pair)

#ifdef GGL_ADAPTED_STD_RANGE_TAG_DEFINED
#error Include only one headerfile to register tag for adapted std:: containers or iterator pair
#endif

#define GGL_ADAPTED_STD_RANGE_TAG_DEFINED



#include <ggl/core/access.hpp>
#include <ggl/core/tag.hpp>
#include <ggl/core/tags.hpp>


namespace ggl
{


#ifndef DOXYGEN_NO_TRAITS_SPECIALIZATIONS
namespace traits
{


template <typename Point>
struct tag<std::pair<Point, Point> >
{
    typedef segment_tag type;
};

template <typename Point>
struct point_type<std::pair<Point, Point> >
{
    typedef Point type;
};

template <typename Point, std::size_t Dimension>
struct indexed_access<std::pair<Point, Point>, 0, Dimension>
{
    typedef typename ggl::coordinate_type<Point>::type coordinate_type;

    static inline coordinate_type get(std::pair<Point, Point> const& s)
    {
        return ggl::get<Dimension>(s.first);
    }

    static inline void set(std::pair<Point, Point>& s, coordinate_type const& value)
    {
        ggl::set<Dimension>(s.first, value);
    }
};


template <typename Point, std::size_t Dimension>
struct indexed_access<std::pair<Point, Point>, 1, Dimension>
{
    typedef typename ggl::coordinate_type<Point>::type coordinate_type;

    static inline coordinate_type get(std::pair<Point, Point> const& s)
    {
        return ggl::get<Dimension>(s.second);
    }

    static inline void set(std::pair<Point, Point>& s, coordinate_type const& value)
    {
        ggl::set<Dimension>(s.second, value);
    }
};


} // namespace traits
#endif // DOXYGEN_NO_TRAITS_SPECIALIZATIONS

} // namespace ggl


#endif // GGL_GEOMETRIES_ADAPTED_STD_PAIR_AS_SEGMENT_HPP

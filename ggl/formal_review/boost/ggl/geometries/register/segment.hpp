// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_GEOMETRIES_REGISTER_SEGMENT_HPP
#define GGL_GEOMETRIES_REGISTER_SEGMENT_HPP


#ifndef DOXYGEN_NO_SPECIALIZATIONS


#define GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_ACCESS(Segment, Point, Index0, Index1) \
template <size_t D> \
struct indexed_access<Segment, min_corner, D> \
{ \
    typedef typename coordinate_type<Point>::type ct; \
    static inline ct get(Segment const& b) \
    { return ggl::get<D>(b. Index0);  } \
    static inline void set(Segment& b, ct const& value) \
    { ggl::set<D>(b. Index0, value);  } \
}; \
template <size_t D> \
struct indexed_access<Segment, max_corner, D> \
{ \
    typedef typename coordinate_type<Point>::type ct; \
    static inline ct get(Segment const& b) \
    { return ggl::get<D>(b. Index1);  } \
    static inline void set(Segment& b, ct const& value) \
    { ggl::set<D>(b. Index1, value);  } \
};


#define GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_ACCESS_TEMPLATIZED(Segment, Index0, Index1) \
template <typename P, size_t D> \
struct indexed_access<Segment<P>, min_corner, D> \
{ \
    typedef typename coordinate_type<P>::type ct; \
    static inline ct get(Segment<P> const& b) \
    { return ggl::get<D>(b. Index0);  } \
    static inline void set(Segment<P>& b, ct const& value) \
    { ggl::set<D>(b. Index0, value);  } \
}; \
template <typename P, size_t D> \
struct indexed_access<Segment<P>, max_corner, D> \
{ \
    typedef typename coordinate_type<P>::type ct; \
    static inline ct get(Segment<P> const& b) \
    { return ggl::get<D>(b. Index1);  } \
    static inline void set(Segment<P>& b, ct const& value) \
    { ggl::set<D>(b. Index1, value);  } \
};


#define GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_ACCESS_4VALUES(Segment, Point, Left, Bottom, Right, Top) \
template <> struct indexed_access<Segment, min_corner, 0> \
{ \
    typedef coordinate_type<Point>::type ct; \
    static inline ct get(Segment const& b) { return b. Left;  } \
    static inline void set(Segment& b, ct const& value) { b. Left = value; } \
}; \
template <> struct indexed_access<Segment, min_corner, 1> \
{ \
    typedef coordinate_type<Point>::type ct; \
    static inline ct get(Segment const& b) { return b. Bottom;  } \
    static inline void set(Segment& b, ct const& value) { b. Bottom = value; } \
}; \
template <> struct indexed_access<Segment, max_corner, 0> \
{ \
    typedef coordinate_type<Point>::type ct; \
    static inline ct get(Segment const& b) { return b. Right;  } \
    static inline void set(Segment& b, ct const& value) { b. Right = value; } \
}; \
template <> struct indexed_access<Segment, max_corner, 1> \
{ \
    typedef coordinate_type<Point>::type ct; \
    static inline ct get(Segment const& b) { return b. Top; } \
    static inline void set(Segment& b, ct const& value) { b. Top = value; } \
};




#define GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_TRAITS(Segment, PointType) \
    template<> struct tag<Segment > { typedef segment_tag type; }; \
    template<> struct point_type<Segment > { typedef PointType type; };

#define GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_TRAITS_TEMPLATIZED(Segment) \
    template<typename P> struct tag<Segment<P> > { typedef segment_tag type; }; \
    template<typename P> struct point_type<Segment<P> > { typedef P type; };

#endif // DOXYGEN_NO_SPECIALIZATIONS



#define GEOMETRY_REGISTER_SEGMENT(Segment, PointType, Index0, Index1) \
namespace ggl { namespace traits {  \
    GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_TRAITS(Segment, PointType) \
    GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_ACCESS(Segment, PointType, Index0, Index1) \
}}


#define GEOMETRY_REGISTER_SEGMENT_TEMPLATIZED(Segment, Index0, Index1) \
namespace ggl { namespace traits {  \
    GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_TRAITS_TEMPLATIZED(Segment) \
    GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_ACCESS_TEMPLATIZED(Segment, Index0, Index1) \
}}

#define GEOMETRY_REGISTER_SEGMENT_2D_4VALUES(Segment, PointType, Left, Bottom, Right, Top) \
namespace ggl { namespace traits {  \
    GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_TRAITS(Segment, PointType) \
    GEOMETRY_DETAIL_SPECIALIZE_SEGMENT_ACCESS_4VALUES(Segment, PointType, Left, Bottom, Right, Top) \
}}



// CONST versions are for segments probably not that common. Postponed.


#endif // GGL_GEOMETRIES_REGISTER_SEGMENT_HPP

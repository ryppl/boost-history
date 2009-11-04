// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_UNION_HPP
#define GGL_ALGORITHMS_UNION_HPP


#include <boost/mpl/if.hpp>

#include <ggl/core/interior_rings.hpp>
#include <ggl/core/reverse_dispatch.hpp>
#include <ggl/geometries/concepts/check.hpp>
#include <ggl/algorithms/overlay/assemble.hpp>
#include <ggl/algorithms/overlay/sort_interior_rings.hpp>
#include <ggl/algorithms/within.hpp>



/*!
\defgroup union union: calculate new geometry containing geometries A or B
\details The union of two geometries A and B is the geometry containing
    all points belong to either of A or B, but no other elements.
\par Source description:
- OGC: Returns a geometric object that represents the Point set union of
    this geometric object with another Geometry.
\see http://en.wikipedia.org/wiki/Union_(set_theory)
\note A union of two rings can result in a polygon having a hole

\par Geometries:
- \b polygon + \b box -> \b polygon(s)
\image html svg_union_polygon_box.png
- \b ring + \b box -> \b polygon(s)
\image html svg_union_ring_box.png
- \b ring + \b ring -> \b polygon(s)
\image html svg_union_ring_ring.png
- \b polygon + \b ring -> \b polygon(s)
\image html svg_union_polygon_ring.png
- combinations above -> \b ring(s).
    <i>If the output is an ouput iterator of rings, holes are omitted</i>

*/

namespace ggl
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace overlay {


// Specializations for "take_one" for union
// "one" should be the outer one, containing the inner one

// for ring and box
template
<
    typename Tag1, typename Geometry1,
    typename Tag2, typename Geometry2,
    typename GeometryOut
>
struct take_if_1_is_in_2<Tag1, Geometry1, Tag2, Geometry2, GeometryOut, 1>
{
    static inline void apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2,
                GeometryOut& out
                )
    {
        ggl::convert(geometry2, out);
    }
};


// spcialize more for polygons
template
<
    typename Tag1, typename Geometry1,
    typename Geometry2,
    typename GeometryOut
>
struct take_if_1_is_in_2<Tag1, Geometry1, polygon_tag, Geometry2, GeometryOut, 1>
{
    static inline void apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2,
                GeometryOut& out
                )
    {
        ggl::convert(exterior_ring(geometry2), out);
    }
};


template
<
    typename Geometry, typename GeometryOut, typename Container
>
struct add_holes<polygon_tag, Geometry, GeometryOut, Container, 1>
{
    static inline void apply(Geometry const& geometry,
                GeometryOut& out,
                Container const& holes
                )
    {
        std::vector<sortable> v;
        sort_interior_rings(holes, v, -1);

        // For a union, if a ring is containing an inner ring, don't take the outer
        // but leave to the inners.
        // If the inner is lying in geometry2, don't add it

        std::size_t const n = boost::size(v);

        for (std::size_t i = 0; i < n; i++)
        {
            // So, only if no inners:
            if (boost::size(v[i].index_of_holes) == 0)
            {
                bool add = v[i].index_of_parent >= 0;
                if (! add)
                {
                    typename ggl::point_type<GeometryOut>::type point;
                    ggl::point_on_border(holes[v[i].index], point);
                    add = ! ggl::within(point, geometry);
                }
                if (add)
                {
                    typename ring_type<GeometryOut>::type hole;
                    ggl::convert(holes[v[i].index], hole);
                    ggl::interior_rings(out).push_back(hole);
                }
            }
        }
    }
};



// "none or both" should be both
template
<
    typename Geometry1, typename Geometry2,
    typename OutputIterator, typename GeometryOut
>
struct take_none_or_both<Geometry1, Geometry2, OutputIterator, GeometryOut, 1>
{
    static inline void apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2,
                OutputIterator out
                )
    {
        convert_and_output<GeometryOut>(geometry1, out);
        convert_and_output<GeometryOut>(geometry2, out);
    }
};


}} // namespace detail::overlay
#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag1, typename Tag2, typename Tag3,
    typename G1, typename G2,
    typename OutputIterator,
    typename GeometryOut
>
struct union_inserter
    : detail::overlay::overlay_and_assemble
        <G1, G2, OutputIterator, GeometryOut, 1>
{
};


template
<
    typename GeometryTag1, typename GeometryTag2, typename GeometryTag3,
    typename Geometry1, typename Geometry2,
    typename OutputIterator, typename GeometryOut
>
struct union_inserter_reversed
{
    static inline OutputIterator apply(Geometry1 const& g1,
            Geometry2 const& g2, OutputIterator out)
    {
        return union_inserter
            <
                GeometryTag2, GeometryTag1, GeometryTag3,
                Geometry2, Geometry1,
                OutputIterator, GeometryOut
            >::apply(g2, g1, out);
    }
};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
    \brief Combines two geometries which each other
    \ingroup union
    \tparam GeometryOut output geometry type, must be specified
    \tparam Geometry1 first geometry type
    \tparam Geometry2 second geometry type
    \tparam OutputIterator output iterator
    \param geometry1 first geometry
    \param geometry2 second geometry
    \param out the output iterator, outputting polygons
    \return the output iterator
*/
template
<
    typename GeometryOut,
    typename Geometry1,
    typename Geometry2,
    typename OutputIterator
>
inline OutputIterator union_inserter(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            OutputIterator out)
{
    concept::check<const Geometry1>();
    concept::check<const Geometry2>();

    return boost::mpl::if_c
        <
            reverse_dispatch<Geometry1, Geometry2>::type::value,
            dispatch::union_inserter_reversed
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<GeometryOut>::type,
                Geometry1,
                Geometry2,
                OutputIterator, GeometryOut
            >,
            dispatch::union_inserter
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<GeometryOut>::type,
                Geometry1,
                Geometry2,
                OutputIterator, GeometryOut
            >
        >::type::apply(geometry1, geometry2, out);
}


} // ggl


#endif //GGL_ALGORITHMS_UNION_HPP

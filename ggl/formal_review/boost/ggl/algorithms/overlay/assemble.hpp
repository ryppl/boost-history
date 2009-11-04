// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_OVERLAY_ASSEMBLE_HPP
#define GGL_ALGORITHMS_OVERLAY_ASSEMBLE_HPP


#include <deque>

#include <ggl/algorithms/overlay/get_intersection_points.hpp>
#include <ggl/algorithms/overlay/merge_intersection_points.hpp>
#include <ggl/algorithms/overlay/adapt_turns.hpp>
#include <ggl/algorithms/overlay/enrich_intersection_points.hpp>
#include <ggl/algorithms/overlay/traverse.hpp>
#include <ggl/algorithms/overlay/sort_interior_rings.hpp>


#include <ggl/algorithms/convert.hpp>
#include <ggl/algorithms/num_points.hpp>
#include <ggl/algorithms/within.hpp>

#include <ggl/algorithms/detail/point_on_border.hpp>

#include <ggl/iterators/range_type.hpp>

namespace ggl
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace overlay {


// Utility function
template <typename GeometryOut, typename Geometry, typename OutputIterator>
void convert_and_output(Geometry const& geometry, OutputIterator& out)
{
    // How can we avoid the double copy here! It is really bad!
    // We have to create a polygon, then copy it to the output iterator.
    // Having an output-vector would have been better: append it to the vector!
    // So output iterators are not that good.

    GeometryOut result;
    ggl::convert(geometry, result);
    *out = result;
    out++;
}




template
<
    typename Tag1, typename Geometry1,
    typename Tag2, typename Geometry2, typename GeometryOut,
    int Direction
>
struct take_if_1_is_in_2
{};


template
<
    int Direction,
    typename Geometry1, typename Geometry2, typename GeometryOut
>
inline bool take_one(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            GeometryOut& out
            )
{
    typename ggl::point_type<Geometry1>::type point;
    ggl::point_on_border(geometry1, point);
    if (ggl::within(point, geometry2))
    {
        take_if_1_is_in_2
            <
                typename tag<Geometry1>::type, Geometry1,
                typename tag<Geometry2>::type, Geometry2,
                GeometryOut, Direction
            >::apply(geometry1, geometry2, out);
        return true;
    }
    return false;
}


template
<
    typename TagOut,
    typename Geometry, typename GeometryOut,
    typename Container,
    int Direction
>
struct add_holes
{
    static inline void apply(Geometry const&, GeometryOut&, Container const& holes)
    {}
};



template
<
    typename Geometry1, typename Geometry2,
    typename OutputIterator, typename GeometryOut,
    int Direction
>
struct take_none_or_both
{
    static inline void apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2,
                OutputIterator out
                )
    {
    }

};


template
<
    typename Tag, typename GeometryOut,
    typename Container, int Direction
>
struct assemble_rings
{
    template <typename OutputIterator>
    static inline void apply(Container const& rings,
                Container const& ni_holes,
                OutputIterator out)
    {}
};


template
<
    typename GeometryOut, typename Container, int Direction
>
struct assemble_rings<polygon_tag, GeometryOut, Container, Direction>
{
    typedef typename boost::range_value<Container>::type ring_type;
    typedef typename ggl::point_type<ring_type>::type point_type;

    template <typename OutputIterator>
    static inline void apply(Container const& rings,
                Container const& ni_holes,
                OutputIterator out)
    {
        std::vector<sortable> sorted_rings;
        sort_interior_rings(rings, sorted_rings, 1);

        // Add polygons, and holes
        for (std::vector<sortable>::const_iterator
                rit = boost::begin(sorted_rings);
            rit != boost::end(sorted_rings);
            ++rit)
        {
            if (rit->index_of_parent == -1)
            {
                GeometryOut result;
                ggl::convert(rings[rit->index], result);

                // Add holes (direction is OK, so no reverse)
                std::vector<int> const& holes = rit->index_of_holes;
                for (std::vector<int>::const_iterator hit = holes.begin();
                        hit != holes.end();
                        ++hit)
                {
                    interior_rings(result).push_back(rings[*hit]);
                }

                // Add also all non-intersecting-inner-holes
                add_holes
                    <
                        polygon_tag,
                        ring_type, GeometryOut, Container,
                        Direction
                    >::apply(rings[rit->index], result, ni_holes);

                *out = result;
                out++;
            }
        }
    }
};


template
<
    typename GeometryOut, typename Container, int Direction
>
struct assemble_rings<ring_tag, GeometryOut, Container, Direction>
{
    typedef typename boost::range_value<Container>::type ring_type;
    typedef typename ggl::point_type<ring_type>::type point_type;

    template <typename OutputIterator>
    static inline void apply(Container const& rings,
                Container const& ni_holes,
                OutputIterator out)
    {
        // Add positive rings, ignore all holes
        for (typename boost::range_const_iterator<Container>::type
            it = boost::begin(rings);
            it != boost::end(rings);
            ++it)
        {
            if (ggl::area(*it) > 0)
            {
                convert_and_output<GeometryOut>(*it, out);
            }
        }
    }
};



template<typename Tag>
struct get_not_intersecting_holes
{
    template
    <
        typename Polygon,
        typename IpContainer, typename HoleContainer
    >
    static inline void apply(Polygon const&, int,
        IpContainer const&, HoleContainer& )
    {
    }
};


template<>
struct get_not_intersecting_holes<polygon_tag>
{
    template
    <
        typename Polygon,
        typename IpContainer, typename HoleContainer
    >
    static inline void apply(Polygon const& polygon, int source_index,
        IpContainer const& ips,
        HoleContainer& holes)
    {
        typedef typename boost::range_value<IpContainer>::type ip_type;
        typedef typename boost::range_const_iterator<IpContainer>::type iterator_type;
        typedef typename ip_type::traversal_vector vector_type;
        typedef typename boost::range_const_iterator<vector_type>::type tvit_type;

        // Declare vector, fill with false
        int const n = num_interior_rings(polygon);
        std::vector<bool> intersecting(n, false);


        // Check all IP's and set corresponding inner rings to "intersecting"
        for (iterator_type it = boost::begin(ips);
            it != boost::end(ips);
            ++it)
        {
           for (tvit_type tvit = boost::begin(it->info);
                tvit != boost::end(it->info);
                ++tvit)
            {
                if (tvit->seg_id.source_index == source_index)
                {
                    int const r = tvit->seg_id.ring_index;
                    if (r >= 0 && r < n)
                    {
                        intersecting[r] = true;
                    }
                }
            }
        }

        typename boost::range_const_iterator
                <
                    typename interior_type<Polygon>::type
                >::type rit = boost::begin(interior_rings(polygon));
        std::vector<bool>::const_iterator iit = boost::begin(intersecting);

        for (;
            rit != boost::end(interior_rings(polygon))
            && iit != boost::end(intersecting);
             ++rit, ++iit)
        {
            if (! (*iit))
            {
                // Don't pushback, it might be of different type.
                holes.resize(holes.size() + 1);
                ggl::convert(*rit, holes.back());
            }
        }
    }
};



template
<
    typename Geometry1, typename Geometry2,
    typename OutputIterator, typename GeometryOut,
    int Direction
>
struct overlay_and_assemble
{
    typedef typename ggl::tag<Geometry1>::type tag1;
    typedef typename ggl::tag<Geometry2>::type tag2;
    typedef typename ggl::tag<GeometryOut>::type tag_out;

    static inline OutputIterator apply(Geometry1 const& geometry1,
                Geometry2 const& geometry2, OutputIterator out)
    {
        if (ggl::num_points(geometry1) == 0
            || ggl::num_points(geometry2) == 0)
        {
            return out;
        }


        typedef typename ggl::point_type<GeometryOut>::type point_type;
        typedef detail::intersection::intersection_point<point_type> ip_type;
        typedef std::deque<ip_type> container_type;

        // "Abuse" rangetype for ringtype: for polygon, it is the type of the
        // exterior ring. For ring, it is the ring itself. That is what is
        // wished here as well.
        typedef typename ggl::range_type<GeometryOut>::type ring_type;

        container_type ips; // intersection points

        bool trivial = ggl::get_intersection_points(geometry1, geometry2, ips);

        // If there are no IP-s, check if one point is in other geometry
        // (both geometries have points, see check above)
        // If yes, take the correct one

        // (which depends on intersection/union)
        // If both are unrelated, take none (intersection) or both (union)

        // If there are disjoint or containing holes:
        // - Hole of A can be within hole of B
        //   - for union: take hole of A
        //   - for intersection: take hole of B
        // - Hole of A can be disjoint from B
        //   - for union: remove those holes
        //   - for intersection: take all those holes
        // -> generic procedure
        // - Collect all holes in one vector
        // - Sort them on decreasing area
        // - Check if one is within a larger one,
        //   - if yes, take one of them
        //   - if no, take all, or none of them

        // Gather non-intersecting holes
        typedef std::vector<ring_type> hole_vector_type;
        hole_vector_type ni_holes;
        get_not_intersecting_holes<tag1>::apply(geometry1, 0, ips, ni_holes);
        get_not_intersecting_holes<tag2>::apply(geometry2, 1, ips, ni_holes);

        if (ips.size() <= 0)
        {
            GeometryOut result;
            if (take_one<Direction>(geometry1, geometry2, result))
            {
                add_holes
                    <
                        tag_out, Geometry1, GeometryOut, hole_vector_type,
                        Direction
                    >::apply(geometry1, result, ni_holes);
                *out = result;
                out++;
            }
            else if (take_one<Direction>(geometry2, geometry1, result))
            {
                add_holes
                    <
                        tag_out,
                        Geometry2, GeometryOut, hole_vector_type,
                        Direction
                    >::apply(geometry2, result, ni_holes);
                *out = result;
                out++;
            }
            else
            {
                take_none_or_both
                    <
                        Geometry1, Geometry2,
                        OutputIterator, GeometryOut,
                        Direction
                    >::apply(geometry1, geometry2, out);
            }
        }
        else
        {
            if (! trivial)
            {
                ggl::merge_intersection_points(ips);

                // If all IP's are removed, they are all collinear or forming
                // an angle which each other.
                // In that case, the inputs are EQUAL.
                // For both intersection and union, just output one.
                if (ips.size() == 0)
                {
                    convert_and_output<GeometryOut>(geometry1, out);
                    return out;
                }

                ggl::adapt_turns(ips);
            }

            ggl::enrich_intersection_points(ips, trivial);


            std::vector<ring_type> v;
            ggl::traverse<ring_type>
                (
                    geometry1,
                    geometry2,
                    Direction,
                    ips,
                    trivial,
                    std::back_inserter(v)
                );

            switch (boost::size(v))
            {
                case 0 : break;
                case 1 :
                    //convert_and_output<GeometryOut>(v.front(), out);
                    {
                        GeometryOut result;
                        ggl::convert(v.front(), result);
                        add_holes
                            <
                                tag_out,
                                ring_type, GeometryOut, hole_vector_type,
                                Direction
                            >::apply(v.front(), result, ni_holes);
                        *out = result;
                        out++;
                    }
                    break;
                default :
                    assemble_rings
                        <
                            tag_out, GeometryOut,
                            hole_vector_type, Direction
                        >::apply(v, ni_holes, out);
            }
        }

        return out;
    }
};


}} // namespace detail::overlay
#endif // DOXYGEN_NO_DETAIL


} // ggl

#endif //GGL_ALGORITHMS_OVERLAY_ASSEMBLE_HPP

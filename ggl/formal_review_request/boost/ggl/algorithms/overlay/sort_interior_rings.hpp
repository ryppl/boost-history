// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_ALGORITHMS_OVERLAY_SORT_INTERIOR_RINGS_HPP
#define GGL_ALGORITHMS_OVERLAY_SORT_INTERIOR_RINGS_HPP

#include <algorithm>
#include <vector>

#include <ggl/algorithms/area.hpp>
#include <ggl/algorithms/within.hpp>
#include <ggl/algorithms/detail/point_on_border.hpp>

namespace ggl
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace overlay {

struct sortable
{
    int index; // zero-based (ZB) index to ring in rings
    double area;
    int index_of_parent; // ZB index to parent-ring
    std::vector<int> index_of_holes; // ZB index to holes

    inline sortable(int i, double a)
        : index(i)
        , area(a)
        , index_of_parent(-1)
    {}

    inline bool operator<(sortable const& other) const
    {
        // sort in reverse order
        return area > other.area;
    }
};



template<typename Container>
void sort_interior_rings(Container const& rings, std::vector<sortable>& sorted_rings, int factor)
{
    typedef typename boost::range_value<Container>::type ring_type;
    typedef typename ggl::point_type<ring_type>::type point_type;

    // Because of the whole followed procedure, the orientations (cw or ccw)
    // of parents and holes are OK, and their areas are positive (parents)
    // or negative (children)

    // Create a copy (without copying geometries), with area (and envelope)
    std::size_t n = 0;
    for (typename boost::range_const_iterator<Container>::type
            it = boost::begin(rings);
        it != boost::end(rings);
        ++it, ++n)
    {
        sorted_rings.push_back(sortable(n, factor * ggl::area(*it)));
    }

    // Sort in reverse order -> largest rings first, holes (neg. area) last
    std::sort(sorted_rings.begin(), sorted_rings.end());

    // Check which holes are in which polygons
    for (std::size_t hole = 1; hole < n; hole++)
    {
        // It is a hole if its area is negative
        if (sorted_rings[hole].area < 0 || factor < 0)
        {
            // So check if it is lying in one of the earlier rings
            bool parent_found = false;
            for (std::size_t parent = 0;
                    parent < hole
                        && ! parent_found
                        && sorted_rings[parent].area > 0;
                    parent++)
            {
                // note that "hole"  is indexed in sorted rings,
                // so make new "h" that is indexed in original rings
                int const h = sorted_rings[hole].index;
                int const p = sorted_rings[parent].index;

                point_type point;
                ggl::point_on_border(rings[h], point);
                if (ggl::within(point, rings[p]))
                {
                    // Update registration
                    sorted_rings[hole].index_of_parent = p;
                    sorted_rings[parent].index_of_holes.push_back(h);
                    parent_found = true;
                }
            }
        }
    }

    /***
    for (std::size_t i = 0; i < n; i++)
    {
        std::cout << "i: " << i
            << " area: " << sorted_rings[i].area
            << " count: " << rings[sorted_rings[i].index].size()
            << " parent: " << sorted_rings[i].index_of_parent
            << " holes: ";
        for (int j = 0; j < boost::size(sorted_rings[i].index_of_holes); j++)
        {
            std::cout << " " << sorted_rings[i].index_of_holes[j];
        }
        std::cout << std::endl;
    }
    ***/
}



}} // namespace detail::overlay
#endif //DOXYGEN_NO_DETAIL


} // namespace ggl

#endif // GGL_ALGORITHMS_OVERLAY_SORT_INTERIOR_RINGS_HPP

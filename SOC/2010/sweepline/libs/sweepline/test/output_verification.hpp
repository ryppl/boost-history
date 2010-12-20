// Boost sweepline library output_verification.hpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef VORONOI_OUTPUT_VERIFICATION
#define VORONOI_OUTPUT_VERIFICATION

#include <map>
#include <vector>

template <typename Point2D>
detail::kOrientation get_orientation(const Point2D &point1,
                                     const Point2D &point2,
                                     const Point2D &point3) {
    typename Point2D::coordinate_type a = (point2.x() - point1.x()) * (point3.y() - point2.y());
    typename Point2D::coordinate_type b = (point2.y() - point1.y()) * (point3.x() - point2.x());
    if (a == b)
        return detail::COLLINEAR;
    return (a < b) ? detail::RIGHT_ORIENTATION : detail::LEFT_ORIENTATION;
}

template <typename Output>
bool verify_half_edge_orientation(const Output &output) {
    typedef typename Output::point_2d_type point_2d_type;
    typename Output::voronoi_edge_const_iterator_type edge_it;
    for (edge_it = output.edge_records().begin(); 
         edge_it != output.edge_records().end(); edge_it++) {
        if (edge_it->is_bounded()) {
            const point_2d_type &site_point = edge_it->cell()->point0();
            const point_2d_type &start_point = edge_it->vertex0()->vertex();
            const point_2d_type &end_point = edge_it->vertex1()->vertex();
            if (get_orientation(start_point, end_point, site_point) != detail::LEFT_ORIENTATION)
                return false;
        }
    }
    return true;
}

template <typename Output>
bool verify_cell_convexity(const Output &output) {
    typedef typename Output::point_2d_type point_2d_type;
    typename Output::voronoi_cell_const_iterator_type cell_it;
    for (cell_it = output.cell_records().begin();
         cell_it != output.cell_records().end(); cell_it++) {
        const typename Output::voronoi_edge_type *edge = cell_it->incident_edge();
        if (edge)
            do {
                if (edge->next()->prev() != edge)
                    return false;
                if (edge->cell() != &(*cell_it))
                    return false;
                if (edge->vertex0() != NULL &&
                    edge->vertex1() != NULL &&
                    edge->vertex1() == edge->next()->vertex0() &&
                    edge->next()->vertex1() != NULL) {
                        const point_2d_type &vertex1 = edge->vertex0()->vertex();
                        const point_2d_type &vertex2 = edge->vertex1()->vertex();
                        const point_2d_type &vertex3 = edge->next()->vertex1()->vertex();
                        if (get_orientation(vertex1, vertex2, vertex3) != detail::LEFT_ORIENTATION)
                            return false;
                }
                edge = edge->next();
            } while(edge != cell_it->incident_edge());
    }
    return true;
}

template <typename Output>
bool verify_incident_edges_ccw_order(const Output &output) {
    typedef typename Output::point_2d_type point_2d_type;
    typedef typename Output::voronoi_edge_type voronoi_edge_type;
    typename Output::voronoi_vertex_const_iterator_type vertex_it;
    for (vertex_it = output.vertex_records().begin();
         vertex_it != output.vertex_records().end(); vertex_it++) {
        if (vertex_it->num_incident_edges() < 3)
            continue;
        const voronoi_edge_type *edge = vertex_it->incident_edge();
        do {
            const voronoi_edge_type *edge_next1 = edge->rot_next();
            const voronoi_edge_type *edge_next2 = edge_next1->rot_next();
            const point_2d_type &site1 = edge->cell()->point0();
            const point_2d_type &site2 = edge_next1->cell()->point0();
            const point_2d_type &site3 = edge_next2->cell()->point0();

            if (get_orientation(site1, site2, site3) != detail::LEFT_ORIENTATION)
                return false;

            edge = edge->rot_next();
        } while (edge != vertex_it->incident_edge());
    }
    return true;
}

template <typename Output>
bool verfiy_no_line_edge_intersections(const Output &output) {
    // Create map from edges with first point less than the second one.
    // Key is the first point of the edge, value is a vector of second points
    // with the same first point.
    typedef typename Output::point_2d_type point_2d_type;
    std::map< point_2d_type, std::vector<point_2d_type> > edge_map;
    typedef typename std::map< point_2d_type, std::vector<point_2d_type> >::const_iterator
        edge_map_iterator;
    typename Output::voronoi_edge_const_iterator_type edge_it;
    for (edge_it = output.edge_records().begin();
         edge_it != output.edge_records().end(); edge_it++) {
        if (edge_it->is_bounded()) {
            const point_2d_type &vertex0 = edge_it->vertex0()->vertex();
            const point_2d_type &vertex1 = edge_it->vertex1()->vertex();
            if (vertex0 < vertex1)
                edge_map[vertex0].push_back(vertex1);
        }
    }

    // Iterate over map of edges and check if there are any intersections.
    // All the edges are stored by the low x value. That's why we iterate
    // left to right checking for intersections between all pairs of edges
    // that overlap in the x dimension.
    // Complexity. Approximately N*sqrt(N). Worst case N^2.
    typedef typename std::vector<point_2d_type>::size_type size_type;
    edge_map_iterator edge_map_it1, edge_map_it2, edge_map_it_bound;
    for (edge_map_it1 = edge_map.begin();
         edge_map_it1 != edge_map.end(); edge_map_it1++) {
        const point_2d_type &point1 = edge_map_it1->first;
        for (size_type i = 0; i < edge_map_it1->second.size(); i++) {
            const point_2d_type &point2 = edge_map_it1->second[i];
            typename Output::coordinate_type min_y1 = std::min(point1.y(), point2.y());
            typename Output::coordinate_type max_y1 = std::max(point1.y(), point2.y());

            // Find the first edge with greater or equal first point.
            edge_map_it_bound = edge_map.lower_bound(point2);

            edge_map_it2 = edge_map_it1;
            edge_map_it2++;
            for (; edge_map_it2 != edge_map_it_bound; edge_map_it2++) {
                const point_2d_type &point3 = edge_map_it2->first;
                for (size_type j = 0; j < edge_map_it2->second.size(); j++) {
                    const point_2d_type &point4 = edge_map_it2->second[j];
                    typename Output::coordinate_type min_y2 = std::min(point3.y(), point4.y());
                    typename Output::coordinate_type max_y2 = std::max(point3.y(), point4.y());
                    
                    // In most cases it is enought to make 
                    // simple intersection check in the y dimension.
                    if (!(max_y1 > min_y2 && max_y2 > min_y1))
                        continue;

                    // Intersection check.
                    if (get_orientation(point1, point2, point3) *
                        get_orientation(point1, point2, point4) == detail::RIGHT_ORIENTATION &&
                        get_orientation(point3, point4, point1) *
                        get_orientation(point3, point4, point2) == detail::RIGHT_ORIENTATION)
                        return false;
                }
            }
        }
    }
    return true;
}

template <typename P>
bool do_intersect(const P &p1, const P &p2, const P &p3, const P &p4) {
    if ((std::max)(p1.y(), p2.y()) < (std::min)(p3.y(), p4.y()) ||
        (std::max)(p3.y(), p4.y()) < (std::min)(p1.y(), p2.y()))
        return false;
    if (p1 == p3)
        return detail::orientation_test(p1, p2, p4) == detail::COLLINEAR;
    if (p2 == p4)
        return detail::orientation_test(p1, p2, p3) == detail::COLLINEAR;
    if (p2 == p3)
        return false;
    if ((get_orientation(p1, p2, p3) * get_orientation(p1, p2, p4) != detail::LEFT_ORIENTATION) &&
        (get_orientation(p3, p4, p1) * get_orientation(p3, p4, p2) != detail::LEFT_ORIENTATION))
        return true;
    return false;
}

template <typename P>
struct segm_comparison {
    bool operator()(const std::pair<P, P> &segm1, const std::pair<P, P> &segm2) const {
        return segm1.first.x() < segm2.first.x();
    }
};

template <typename P>
void remove_intersections(std::vector< std::pair<P, P> > &segm_vec) {
    for (int i = 0; i < static_cast<int>(segm_vec.size()); i++) {
        if (segm_vec[i].first > segm_vec[i].second) {
            (std::swap(segm_vec[i].first, segm_vec[i].second));
        }
    }
    sort(segm_vec.begin(), segm_vec.end());
    std::vector< std::pair<P, P> > dest_vec;
    typename std::vector< std::pair<P, P> >::iterator it, b_it, l_bound;
    for (it = segm_vec.begin(); it != segm_vec.end(); it++) {
        std::pair<P, P> bound = std::make_pair(it->second, it->second);
        l_bound = upper_bound(segm_vec.begin(), segm_vec.end(), bound, segm_comparison<P>());
        bool add = true;
        for (b_it = it + 1; b_it != l_bound; b_it++) {
            if (do_intersect(it->first, it->second, b_it->first, b_it->second)) {
                add = false;
                break;
            }
        }
        if (add) {
            dest_vec.push_back(*it);
        }
    }
    segm_vec.swap(dest_vec);
}

enum kVerification {
    HALF_EDGE_ORIENTATION = 1,
    CELL_CONVEXITY = 2,
    INCIDENT_EDGES_CCW_ORDER = 4,
    NO_HALF_EDGE_INTERSECTIONS = 8,
    FAST_VERIFICATION = 7,
    COMPLETE_VERIFICATION = 15,
};

template <typename Output>
bool verify_output(const Output &output, kVerification mask) {
    bool result = true;
    if (mask & HALF_EDGE_ORIENTATION)
        result &= verify_half_edge_orientation(output);
    if (mask & CELL_CONVEXITY)
        result &= verify_cell_convexity(output);
    if (mask & INCIDENT_EDGES_CCW_ORDER)
        result &= verify_incident_edges_ccw_order(output);
    if (mask & NO_HALF_EDGE_INTERSECTIONS)
        result &= verfiy_no_line_edge_intersections(output);
    return result;
}

#endif

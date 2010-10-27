// Boost sweepline library voronoi_output_verification.hpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef VORONOI_OUTPUT_VERIFICATION
#define VORONOI_OUTPUT_VERIFICATION

#include <map>
#include <vector>

enum kOrientation {
    RIGHT_ORIENTATION = -1,
    COLLINEAR = 0,
    LEFT_ORIENTATION = 1,
};

template <typename Point2D>
kOrientation get_orientation(const Point2D &point1,
                     const Point2D &point2,
                     const Point2D &point3) {
    typename Point2D::coordinate_type a = (point2.x() - point1.x()) * (point3.y() - point2.y());
    typename Point2D::coordinate_type b = (point2.y() - point1.y()) * (point3.x() - point2.x());
    if (a == b)
        return COLLINEAR;
    return (a < b) ? RIGHT_ORIENTATION : LEFT_ORIENTATION;
}

template <typename Output>
bool verify_half_edge_orientation(const Output &output) {
    typedef typename Output::Point2D Point2D;
    typename Output::voronoi_edge_const_iterator_type edge_it;
    for (edge_it = output.edge_records.begin(); 
         edge_it != output.edge_records.end(); edge_it++) {
        if (edge_it->start_point != NULL && edge_it->end_point != NULL) {
            const Point2D &site_point = edge_it->cell->get_point0();
            const Point2D &start_point = edge_it->start_point->vertex;
            const Point2D &end_point = edge_it->end_point->vertex;
            if (get_orientation(start_point, end_point, site_point) != LEFT_ORIENTATION)
                return false;
        }
    }
    return true;
}

template <typename Output>
bool verify_cell_convexity(const Output &output) {
    typedef typename Output::Point2D Point2D;
    typename Output::voronoi_cell_const_iterator_type cell_it;
    for (cell_it = output.cell_records.begin();
         cell_it != output.cell_records.end(); cell_it++) {
        const typename Output::voronoi_edge_type *edge = cell_it->incident_edge;
        if (edge)
            do {
                if (edge->next->prev != edge)
                    return false;
                if (edge->cell != &(*cell_it))
                    return false;
                if (edge->start_point != NULL &&
                    edge->end_point != NULL &&
                    edge->end_point == edge->next->start_point &&
                    edge->next->end_point != NULL) {
                        const Point2D &vertex1 = edge->start_point->vertex;
                        const Point2D &vertex2 = edge->end_point->vertex;
                        const Point2D &vertex3 = edge->next->end_point->vertex;
                        if (get_orientation(vertex1, vertex2, vertex3) != LEFT_ORIENTATION)
                            return false;
                }
                edge = edge->next;
            } while(edge != cell_it->incident_edge);
    }
    return true;
}

template <typename Output>
bool verify_incident_edges_ccw_order(const Output &output) {
    typedef typename Output::Point2D Point2D;
    typename Output::voronoi_vertex_const_iterator_type vertex_it;
    for (vertex_it = output.vertex_records.begin();
         vertex_it != output.vertex_records.end(); vertex_it++) {
        if (vertex_it->num_incident_edges < 3)
            continue;
        typename Output::voronoi_edge_type *edge = vertex_it->incident_edge;
        do {
            typename Output::voronoi_edge_type *edge_next1 = edge->rot_next;
            typename Output::voronoi_edge_type *edge_next2 = edge_next1->rot_next;
            const Point2D &site1 = edge->cell->get_point0();
            const Point2D &site2 = edge_next1->cell->get_point0();
            const Point2D &site3 = edge_next2->cell->get_point0();

            if (get_orientation(site1, site2, site3) != LEFT_ORIENTATION)
                return false;

            edge = edge->rot_next;
        } while (edge != vertex_it->incident_edge);
    }
    return true;
}

template <typename Output>
bool verfiy_no_half_edge_intersections(const Output &output) {
    // Create map from edges with first point less than the second one.
    // Key is the first point of the edge, value is a vector of second points
    // with the same first point.
    typedef typename Output::Point2D Point2D;
    std::map< Point2D, std::vector<Point2D> > edge_map;
    typedef typename std::map< Point2D, std::vector<Point2D> >::const_iterator 
        edge_map_iterator;
    typename Output::voronoi_edge_const_iterator_type edge_it;
    for (edge_it = output.edge_records.begin();
         edge_it != output.edge_records.end(); edge_it++) {
        if (edge_it->start_point != NULL && edge_it->end_point != NULL) {
            const Point2D &start_point = edge_it->start_point->vertex;
            const Point2D &end_point = edge_it->end_point->vertex;
            if (start_point < end_point)
                edge_map[start_point].push_back(end_point);
        }
    }

    // Iterate over map of edges and check if there are any intersections.
    // All the edges are stored by the low x value. That's why we iterate
    // left to right checking for intersections between all pairs of edges
    // that overlap in the x dimension.
    // Complexity. Approximately N*sqrt(N). Worst case N^2.
    typedef typename std::vector<Point2D>::size_type size_type;
    edge_map_iterator edge_map_it1, edge_map_it2, edge_map_it_bound;
    for (edge_map_it1 = edge_map.begin();
         edge_map_it1 != edge_map.end(); edge_map_it1++) {
        const Point2D &point1 = edge_map_it1->first;
        for (size_type i = 0; i < edge_map_it1->second.size(); i++) {
            const Point2D &point2 = edge_map_it1->second[i];
            typename Output::coordinate_type min_y1 = std::min(point1.y(), point2.y());
            typename Output::coordinate_type max_y1 = std::max(point1.y(), point2.y());

            // Find the first edge with greater or equal first point.
            edge_map_it_bound = edge_map.lower_bound(point2);

            edge_map_it2 = edge_map_it1;
            edge_map_it2++;
            for (; edge_map_it2 != edge_map_it_bound; edge_map_it2++) {
                const Point2D &point3 = edge_map_it2->first;
                for (size_type j = 0; j < edge_map_it2->second.size(); j++) {
                    const Point2D &point4 = edge_map_it2->second[j];
                    typename Output::coordinate_type min_y2 = std::min(point3.y(), point4.y());
                    typename Output::coordinate_type max_y2 = std::max(point3.y(), point4.y());
                    
                    // In most cases it is enought to make 
                    // simple intersection check in the y dimension.
                    if (!(max_y1 > min_y2 && max_y2 > min_y1))
                        continue;

                    // Intersection check.
                    if (get_orientation(point1, point2, point3) *
                        get_orientation(point1, point2, point4) == -1 &&
                        get_orientation(point3, point4, point1) *
                        get_orientation(point3, point4, point2) == -1)
                        return false;
                }
            }
        }
    }
    return true;
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
        result &= verfiy_no_half_edge_intersections(output);
    return result;
}

#endif

// Boost sweepline/voronoi_output.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_OUTPUT
#define BOOST_SWEEPLINE_VORONOI_OUTPUT

#include <list>
#include <map>
#include <vector>

namespace boost {
namespace sweepline {

    template <typename T>
    struct point_2d {
    public:
        typedef T coordinate_type;

        point_2d() {}

        point_2d(T x, T y) {
            x_ = x;
            y_ = y;
        }

        bool operator==(const point_2d &point) const {
            return (this->x_ == point.x()) && (this->y_ == point.y());
        }

        bool operator!=(const point_2d &point) const {
            return (this->x_ != point.x()) || (this->y_ != point.y());
        }

        // This comparator actually defines sweepline movement direction.
        bool operator<(const point_2d &point) const {
            // Sweepline sweeps from left to right.
            if (this->x_ != point.x_)
                return this->x_ < point.x_;
            return this->y_ < point.y_;
        }

        bool operator<=(const point_2d &point) const {
            return !(point < (*this));
        }

        bool operator>(const point_2d &point) const {
            return point < (*this);
        }

        bool operator>=(const point_2d &point) const {
            return !((*this) < point);
        }

        coordinate_type x() const {
            return this->x_;
        }

        coordinate_type y() const {
            return this->y_;
        }

        void x(coordinate_type x) {
            x_ = x;
        }

        void y(coordinate_type y) {
            y_ = y;
        }

    private:
        coordinate_type x_;
        coordinate_type y_;
    };

    template <typename T>
    point_2d<T> make_point_2d(T x, T y) {
        return point_2d<T>(x,y);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT TYPES ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Bounding rectangle data structure.
    template <typename T>
    struct BRect {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;

        coordinate_type x_min;
        coordinate_type y_min;
        coordinate_type x_max;
        coordinate_type y_max;

        BRect() {}

        BRect(const Point2D &p1, const Point2D &p2) {
            x_min = (std::min)(p1.x(), p2.x());
            y_min = (std::min)(p1.y(), p2.y());
            x_max = (std::max)(p1.x(), p2.x());
            y_max = (std::max)(p1.y(), p2.y());
        }

        BRect(coordinate_type x_mn, coordinate_type y_mn,
              coordinate_type x_mx, coordinate_type y_mx) {
             x_min = (std::min)(x_mn, x_mx);
             y_min = (std::min)(y_mn, y_mx);
             x_max = (std::max)(x_mn, x_mx);
             y_max = (std::max)(y_mn, y_mx);
        }

        void update(const Point2D &p) {
            x_min = (std::min)(x_min, p.x());
            y_min = (std::min)(y_min, p.y());
            x_max = (std::max)(x_max, p.x());
            y_max = (std::max)(y_max, p.y());
        }

        bool contains(const Point2D &p) const {
            return p.x() > x_min && p.x() < x_max && p.y() > y_min && p.y() < y_max;
        }

        bool valid() const {
            return (x_min < x_max) && (y_min < y_max);
        }
    };

    template <typename T>
    struct half_edge_clipped;

    // Voronoi record data structure. May represent voronoi cell or
    // voronoi vertex. Contains pointer to any incident edge, point
    // coordinates and number of incident edges.
    template <typename T>
    struct voronoi_record_clipped {
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;

        half_edge_clipped<coordinate_type> *incident_edge;
        Point2D voronoi_point;
        int num_incident_edges;

        voronoi_record_clipped(const Point2D &point, half_edge_clipped<coordinate_type>* edge) :
            incident_edge(edge),
            voronoi_point(point),
            num_incident_edges(0) {}
    };

    // Half-edge data structure. Represents voronoi edge.
    // Contains: 1) pointer to cell records;
    //           2) pointers to start/end vertices of half-edge;
    //           3) pointers to previous/next half-edges(CCW);
    //           4) pointers to previous/next half-edges rotated
    //              around start point;
    //           5) pointer to twin half-edge.
    template <typename T>
    struct half_edge_clipped {
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef voronoi_record_clipped<coordinate_type> voronoi_record_type;
        typedef half_edge_clipped<coordinate_type> half_edge_type;

        voronoi_record_type *cell;
        voronoi_record_type *start_point;
        voronoi_record_type *end_point;
        half_edge_type *prev;
        half_edge_type *next;
        half_edge_type *rot_prev;
        half_edge_type *rot_next;
        half_edge_type *twin;

        half_edge_clipped() :
            cell(NULL),
            start_point(NULL),
            end_point(NULL),
            prev(NULL),
            next(NULL),
            rot_prev(NULL),
            rot_next(NULL),
            twin(NULL) {}
    };

    template <typename T>
    class voronoi_output_clipped {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef voronoi_record_clipped<coordinate_type> voronoi_record_type;
        typedef std::list<voronoi_record_type> voronoi_records_type;
        typedef typename voronoi_records_type::iterator voronoi_iterator_type;
        typedef typename voronoi_records_type::const_iterator voronoi_const_iterator_type;
        typedef half_edge_clipped<coordinate_type> edge_type;
        typedef std::list<edge_type> voronoi_edges_type;
        typedef typename voronoi_edges_type::iterator edges_iterator_type;
        typedef typename voronoi_edges_type::const_iterator edges_const_iterator_type;

        voronoi_output_clipped() {
            num_cell_records_ = 0;
            num_vertex_records_ = 0;
            num_edges_ = 0;
        }

        void reset() {
            cell_records_.clear();
            vertex_records_.clear();
            edges_.clear();

            num_cell_records_ = 0;
            num_vertex_records_ = 0;
            num_edges_ = 0;
        }

        void set_bounding_rectangle(const BRect<coordinate_type> &brect) {
            brect_ = brect;
        }

        const BRect<coordinate_type> &get_bounding_rectangle() {
            return brect_;
        }

        voronoi_record_type &add_vertex(const Point2D &vertex_point) {
            vertex_records_.push_back(voronoi_record_type(vertex_point, NULL));
            num_vertex_records_++;
            return vertex_records_.back();
        }

        voronoi_record_type &add_boundary_vertex(const Point2D &boundary_point) {
            vertex_records_.push_back(voronoi_record_type(boundary_point, NULL));
            return vertex_records_.back();
        }

        edge_type &add_edge() {
            edges_.push_back(edge_type());
            num_edges_++;
            return edges_.back();
        }

        voronoi_record_type &add_cell(const Point2D &site_point) {
            cell_records_.push_back(voronoi_record_type(site_point, NULL));
            num_cell_records_++;
            return cell_records_.back();
        }

        void pop_cell() {
            cell_records_.pop_back();
            num_cell_records_--;
        }

        const voronoi_records_type &get_voronoi_cells() const {
            return cell_records_;
        }

        const voronoi_records_type &get_voronoi_vertices() const {
            return vertex_records_;
        }

        const voronoi_edges_type &get_voronoi_edges() const {
            return edges_;
        }

        int get_num_voronoi_cells() const {
            return num_cell_records_;
        }

        int get_num_voronoi_vertices() const {
            return num_vertex_records_;
        }

        int get_num_voronoi_edges() const {
            return num_edges_ / 2;
        }

        // Check correct orientation of each half_edge.
        bool half_edge_orientation_check() const {
            typename std::list<edge_type>::const_iterator edge_it;
            for (edge_it = edges_.begin(); edge_it != edges_.end(); edge_it++) {
                if (edge_it->start_point != NULL && edge_it->end_point != NULL) {
                    const Point2D &site = edge_it->cell->voronoi_point;
                    const Point2D &start_point = edge_it->start_point->voronoi_point;
                    const Point2D &end_point = edge_it->end_point->voronoi_point;
                    if (check_orientation(start_point, end_point, site) != LEFT_ORIENTATION)
                        return false;
                }
            }
            return true;
        }

        // Check if boundary of each cell is convex.
        bool cell_convexity_check() const {
            voronoi_const_iterator_type cell_it;
            for (cell_it = cell_records_.begin(); cell_it != cell_records_.end(); cell_it++) {
                const edge_type *edge = cell_it->incident_edge;
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
                            const Point2D &vertex1 = edge->start_point->voronoi_point;
                            const Point2D &vertex2 = edge->end_point->voronoi_point;
                            const Point2D &vertex3 = edge->next->end_point->voronoi_point;
                            if (check_orientation(vertex1, vertex2, vertex3) != LEFT_ORIENTATION)
                                return false;
                        }
                        edge = edge->next;
                    } while(edge != cell_it->incident_edge);
            }
            return true;
        }

        // Check CCW ordering of incident edges of voronoi vertices.
        bool incident_edges_order_check() const {
            voronoi_const_iterator_type vertex_it;
            for (vertex_it = vertex_records_.begin();
                 vertex_it != vertex_records_.end(); vertex_it++) {
                if (vertex_it->num_incident_edges < 3)
                    continue;
                edge_type *edge = vertex_it->incident_edge;
                do {
                    edge_type *edge_next1 = edge->rot_next;
                    edge_type *edge_next2 = edge_next1->rot_next;
                    const Point2D &site1 = edge->cell->voronoi_point;
                    const Point2D &site2 = edge_next1->cell->voronoi_point;
                    const Point2D &site3 = edge_next2->cell->voronoi_point;

                    if (check_orientation(site1, site2, site3) != LEFT_ORIENTATION)
                        return false;

                    edge = edge->rot_next;
                } while (edge != vertex_it->incident_edge);
            }
            return true;
        }

        // Check if any two half_edges intersect not at the end point.
        bool half_edges_intersection_check() const {
            // Create map from edges with first point less than the second one.
            // Key is the first point of the edge, value is a vector of second points
            // with the same first point.
            std::map< Point2D, std::vector<Point2D> > edge_map;
            typedef typename std::map< Point2D, std::vector<Point2D> >::const_iterator 
                edge_map_iterator;
            typename std::list<edge_type>::const_iterator edge_it;
            for (edge_it = edges_.begin(); edge_it != edges_.end(); edge_it++) {
                if (edge_it->start_point != NULL && edge_it->end_point != NULL) {
                    const Point2D &start_point = edge_it->start_point->voronoi_point;
                    const Point2D &end_point = edge_it->end_point->voronoi_point;
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
                    coordinate_type min_y1 = std::min(point1.y(), point2.y());
                    coordinate_type max_y1 = std::max(point1.y(), point2.y());

                    // Find the first edge with greater or equal first point.
                    edge_map_it_bound = edge_map.lower_bound(point2);

                    edge_map_it2 = edge_map_it1;
                    edge_map_it2++;
                    for (; edge_map_it2 != edge_map_it_bound; edge_map_it2++) {
                        const Point2D &point3 = edge_map_it2->first;
                        for (size_type j = 0; j < edge_map_it2->second.size(); j++) {
                            const Point2D &point4 = edge_map_it2->second[j];
                            coordinate_type min_y2 = std::min(point3.y(), point4.y());
                            coordinate_type max_y2 = std::max(point3.y(), point4.y());
                            
                            // In most cases it is enought to make 
                            // simple intersection check in the y dimension.
                            if (!(max_y1 > min_y2 && max_y2 > min_y1))
                                continue;

                            // Intersection check.
                            if (check_orientation(point1, point2, point3) *
                                check_orientation(point1, point2, point4) == -1 &&
                                check_orientation(point3, point4, point1) *
                                check_orientation(point3, point4, point2) == -1)
                                return false;
                        }
                    }
                }
            }
            return true;
        }

        // Make a few output checks.
        bool check() const {
            return half_edge_orientation_check() &&
                   cell_convexity_check() &&
                   incident_edges_order_check() &&
                   half_edges_intersection_check();
        }

    private:
        enum kOrientation {
            LEFT_ORIENTATION = 1,
            RIGHT_ORIENTATION = -1,
            collinear = 0,
        };

        int check_orientation(const Point2D &point1,
                              const Point2D &point2,
                              const Point2D &point3) const {
            coordinate_type a = (point2.x() - point1.x()) * (point3.y() - point2.y());
            coordinate_type b = (point2.y() - point1.y()) * (point3.x() - point2.x());
            if (a > b)
                return LEFT_ORIENTATION;
            else if (a < b)
                return RIGHT_ORIENTATION;
            return collinear;
        }

        voronoi_records_type cell_records_;
        voronoi_records_type vertex_records_;
        voronoi_edges_type edges_;
        
        int num_cell_records_;
        int num_vertex_records_;
        int num_edges_;

        BRect<coordinate_type> brect_;

        // Disallow copy constructor and operator=
        voronoi_output_clipped(const voronoi_output_clipped&);
        void operator=(const voronoi_output_clipped&);
    };

} // sweepline
} // boost

#endif

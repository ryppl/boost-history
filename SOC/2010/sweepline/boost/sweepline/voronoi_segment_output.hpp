// Boost sweepline/voronoi_segment_output.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SEGMENT_OUTPUT
#define BOOST_SWEEPLINE_VORONOI_SEGMENT_OUTPUT

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

        bool is_valid() const {
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

    private:
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

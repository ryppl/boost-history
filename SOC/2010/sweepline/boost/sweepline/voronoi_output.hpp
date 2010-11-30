// Boost sweepline/voronoi_output.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_OUTPUT
#define BOOST_SWEEPLINE_VORONOI_OUTPUT

namespace boost {
namespace sweepline {

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT TYPES ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Forward declarations.
    namespace detail {
        template <typename T>
        class site_event;

        template <typename T>
        class circle_event;

        template <typename T>
        class epsilon_robust_comparator;

        template <typename T>
        class voronoi_builder;
    }

    template <typename T>
    class voronoi_cell;

    template <typename T>
    class voronoi_edge;

    template <typename T>
    class voronoi_output;

    // Represents 2D point.
    template <typename T>
    class point_2d {
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

    // Bounding rectangle data structure.
    template <typename T>
    class BRect {
    public:
        typedef T coordinate_type;

        BRect() {}

        BRect(const point_2d<T> &p1, const point_2d<T> &p2) {
            x_min_ = (std::min)(p1.x(), p2.x());
            y_min_ = (std::min)(p1.y(), p2.y());
            x_max_ = (std::max)(p1.x(), p2.x());
            y_max_ = (std::max)(p1.y(), p2.y());
        }

        BRect(coordinate_type x_mn, coordinate_type y_mn,
              coordinate_type x_mx, coordinate_type y_mx) {
             x_min_ = (std::min)(x_mn, x_mx);
             y_min_ = (std::min)(y_mn, y_mx);
             x_max_ = (std::max)(x_mn, x_mx);
             y_max_ = (std::max)(y_mn, y_mx);
        }

        void update(const point_2d<T> &p) {
            x_min_ = (std::min)(x_min_, p.x());
            y_min_ = (std::min)(y_min_, p.y());
            x_max_ = (std::max)(x_max_, p.x());
            y_max_ = (std::max)(y_max_, p.y());
        }

        bool contains(const point_2d<T> &p) const {
            return p.x() > x_min_ && p.x() < x_max_ && p.y() > y_min_ && p.y() < y_max_;
        }

        bool is_valid() const {
            return (x_min_ < x_max_) && (y_min_ < y_max_);
        }

        coordinate_type x_min() const {
            return x_min_;
        }

        coordinate_type y_min() const {
            return y_min_;
        }

        coordinate_type x_max() const {
            return x_max_;
        }

        coordinate_type y_max() const {
            return y_max_;
        }

    private:
        coordinate_type x_min_;
        coordinate_type y_min_;
        coordinate_type x_max_;
        coordinate_type y_max_;
    };

    // Contains output visualization tools.
    template <typename T>
    class voronoi_helper {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;

        enum kEdgeType {
            SEGMENT = 0,
            RAY = 1,
            LINE = 2,
        };

        // Find edge(segment, ray, line) rectangle intersetion points.
        static bool find_intersections(const Point2D &origin, const Point2D &direction,
                                       kEdgeType edge_type, const BRect<coordinate_type> &brect,
                                       std::vector< Point2D > &intersections) {
            coordinate_type half = static_cast<coordinate_type>(0.5);

            // Find rectangle center.
            coordinate_type center_x = (brect.x_min() + brect.x_max()) * half;
            coordinate_type center_y = (brect.y_min() + brect.y_max()) * half;

            // Find rectangle half-diagonal vector.
            coordinate_type len_x = brect.x_max() - center_x;
            coordinate_type len_y = brect.y_max() - center_y;
            
            // Find distance between origin and center of rectangle.
            coordinate_type diff_x = origin.x() - center_x;
            coordinate_type diff_y = origin.y() - center_y;
            
            // Find direction perpendicular to the current.
            coordinate_type perp_x = direction.y();
            coordinate_type perp_y = -direction.x();

            // Compare projections of distances.
            coordinate_type lexpr = magnitude(perp_x * diff_x + perp_y * diff_y);
            coordinate_type rexpr = magnitude(perp_x * len_x) + magnitude(perp_y * len_y);

            // Intersection check.
            if (lexpr > rexpr)
                return false;

            // Intersection parameters:
            // origin + fT[i] * direction = intersections point, i = 0 .. 1.
            bool fT0_used = false;
            bool fT1_used = false;
            coordinate_type fT0 = 0;
            coordinate_type fT1 = 0;

            // Find intersections with lines going through sides of a bounding rectangle.
            clip_line(+direction.x(), -diff_x - len_x, fT0_used, fT1_used, fT0, fT1);
            clip_line(-direction.x(), +diff_x - len_x, fT0_used, fT1_used, fT0, fT1);
            clip_line(+direction.y(), -diff_y - len_y, fT0_used, fT1_used, fT0, fT1);
            clip_line(-direction.y(), +diff_y - len_y, fT0_used, fT1_used, fT0, fT1);

            if (fT0_used && check_extent(fT0, edge_type))
                intersections.push_back(make_point_2d(origin.x() + fT0 * direction.x(),
                                                      origin.y() + fT0 * direction.y()));
            if (fT1_used && fT0 != fT1 && check_extent(fT1, edge_type))
                intersections.push_back(make_point_2d(origin.x() + fT1 * direction.x(),
                                                      origin.y() + fT1 * direction.y()));
            return fT0_used && fT1_used;
        }

        static void fill_intermediate_points(Point2D point_site,
                                             Point2D segment_site_start,
                                             Point2D segment_site_end,
                                             std::vector<Point2D> &intermediate_points) {
            int num_inter_points = get_intermediate_points_count(
                intermediate_points[0], intermediate_points[1]);
            if (num_inter_points <= 0 ||
                point_site == segment_site_start ||
                point_site == segment_site_end) {
                return;
            }
            intermediate_points.reserve(2 + num_inter_points);
            coordinate_type segm_vec_x = segment_site_end.x() - segment_site_start.x();
            coordinate_type segm_vec_y = segment_site_end.y() - segment_site_start.y();
            coordinate_type sqr_segment_length = segm_vec_x * segm_vec_x + segm_vec_y * segm_vec_y;
            coordinate_type projection_start =
                get_point_projection(intermediate_points[0], segment_site_start, segment_site_end);
            coordinate_type projection_end =
                get_point_projection(intermediate_points[1], segment_site_start, segment_site_end);
            coordinate_type step = (projection_end - projection_start) *
                          sqr_segment_length / (num_inter_points + 1);
            coordinate_type point_vec_x = point_site.x() - segment_site_start.x();
            coordinate_type point_vec_y = point_site.y() - segment_site_start.y();
            coordinate_type point_rot_x = segm_vec_x * point_vec_x + segm_vec_y * point_vec_y;
            coordinate_type point_rot_y = segm_vec_x * point_vec_y - segm_vec_y * point_vec_x;
            coordinate_type projection_cur_x = projection_start * sqr_segment_length + step;
            point_2d<T> last_point = intermediate_points.back();
            intermediate_points.pop_back();
            for (int i = 0; i < num_inter_points; i++, projection_cur_x += step) {
                coordinate_type inter_rot_x = projection_cur_x;
                coordinate_type inter_rot_y =
                    ((inter_rot_x - point_rot_x) * (inter_rot_x - point_rot_x) +
                     point_rot_y * point_rot_y) / (2.0 * point_rot_y);
                coordinate_type new_point_x =
                    (segm_vec_x * inter_rot_x - segm_vec_y * inter_rot_y) /
                    sqr_segment_length + segment_site_start.x();
                coordinate_type new_point_y =
                    (segm_vec_x * inter_rot_y + segm_vec_y * inter_rot_x) /
                    sqr_segment_length + segment_site_start.y();
                intermediate_points.push_back(make_point_2d(new_point_x, new_point_y));
            }
            intermediate_points.push_back(last_point);
        }

        static BRect<coordinate_type> get_view_rectangle(const BRect<coordinate_type> &brect) {
            coordinate_type x_len = (brect.x_max() - brect.x_min());
            coordinate_type y_len = (brect.y_max() - brect.y_min());
            coordinate_type x_mid = (brect.x_max() + brect.x_min()) / 2;
            coordinate_type y_mid = (brect.y_max() + brect.y_min()) / 2;
            coordinate_type offset = x_len;
            if (offset < y_len)
                offset = y_len;
            if (offset == 0.0)
                offset = 0.5;
            BRect<coordinate_type> new_brect(x_mid - offset, y_mid - offset,
                                             x_mid + offset, y_mid + offset);
            return new_brect;
        }

        static std::vector< point_2d<coordinate_type> > get_intermediate_points(
            const voronoi_edge<coordinate_type> *edge, const BRect<T> &brect) {
            const voronoi_cell<coordinate_type> *cell1 = edge->cell();
            const voronoi_cell<coordinate_type> *cell2 = edge->twin()->cell();
            std::vector<Point2D> edge_points;
            if (!(cell1->contains_segment() ^ cell2->contains_segment())) {
                if (edge->vertex0() != NULL && edge->vertex1() != NULL) {
                    edge_points.push_back(edge->vertex0()->vertex());
                    edge_points.push_back(edge->vertex1()->vertex());
                } else {
                    const Point2D &site1 = cell1->get_point0();
                    const Point2D &site2 = cell2->get_point0();
                    Point2D origin((site1.x() + site2.x()) * static_cast<coordinate_type>(0.5),
                                   (site1.y() + site2.y()) * static_cast<coordinate_type>(0.5));
                    Point2D direction(site1.y() - site2.y(), site2.x() - site1.x());
                    find_intersections(origin, direction, LINE, brect, edge_points);
                    if (edge->vertex1() != NULL)
                        edge_points[1] = edge->vertex1()->vertex();
                    if (edge->vertex0() != NULL)
                        edge_points[0] = edge->vertex0()->vertex();
                }
            } else {
                const Point2D &point1 = (cell1->contains_segment()) ?
                    cell2->get_point0() : cell1->get_point0();
                const Point2D &point2 = (cell1->contains_segment()) ?
                    cell1->get_point0() : cell2->get_point0();
                const Point2D &point3 = (cell1->contains_segment()) ?
                    cell1->get_point1() : cell2->get_point1();
                if (edge->vertex0() != NULL && edge->vertex1() != NULL) {
                    edge_points.push_back(edge->vertex0()->vertex());
                    edge_points.push_back(edge->vertex1()->vertex());
                    fill_intermediate_points(point1, point2, point3, edge_points);
                } else {
                    coordinate_type dir_x = (cell1->contains_segment() ^ (point1 == point3)) ?
                        point3.y() - point2.y() : point2.y() - point3.y();
                    coordinate_type dir_y = (cell1->contains_segment() ^ (point1 == point3)) ?
                        point2.x() - point3.x() : point3.x() - point2.x();
                    Point2D direction(dir_x, dir_y);
                    find_intersections(point1, direction, LINE, brect, edge_points);
                    if (edge->vertex1() != NULL)
                        edge_points[1] = edge->vertex1()->vertex();
                    if (edge->vertex0() != NULL)
                        edge_points[0] = edge->vertex0()->vertex();
                }
            }
            return edge_points;
        }
    
    private:
        voronoi_helper();

        static bool check_extent(coordinate_type extent, kEdgeType etype) {
            switch (etype) {
                case SEGMENT: return extent >= static_cast<coordinate_type>(0.0) &&
                                     extent <= static_cast<coordinate_type>(1.0);
                case RAY: return extent >= static_cast<coordinate_type>(0.0);
                case LINE: return true;
            }
            return true;
        }

        static coordinate_type magnitude(coordinate_type value) {
            if (value >= static_cast<coordinate_type>(0.0))
                return value;
            return -value;
        }

        static bool clip_line(coordinate_type denom,
                              coordinate_type numer,
                              bool &fT0_used, bool &fT1_used,
                              coordinate_type &fT0,
                              coordinate_type &fT1) {
            if (denom > static_cast<coordinate_type>(0.0)) {
                if (fT1_used && numer > denom * fT1)
                    return false;
                if (!fT0_used || numer > denom * fT0) {
                    fT0_used = true;
                    fT0 = numer / denom;
                }
                return true;
            } else if (denom < static_cast<coordinate_type>(0.0)) {
                if (fT0_used && numer > denom * fT0)
                    return false;
                if (!fT1_used || numer > denom * fT1) {
                    fT1_used = true;
                    fT1 = numer / denom;
                }
                return true;
            }
            return false;
        }

        static coordinate_type get_point_projection(const Point2D &point,
                                                    const Point2D &segment_start,
                                                    const Point2D &segment_end) {
            coordinate_type segment_vec_x = segment_end.x() - segment_start.x();
            coordinate_type segment_vec_y = segment_end.y() - segment_start.y();
            coordinate_type point_vec_x = point.x() - segment_start.x();
            coordinate_type point_vec_y = point.y() - segment_start.y();
            coordinate_type sqr_segment_length = segment_vec_x * segment_vec_x +
                                                 segment_vec_y * segment_vec_y;
            coordinate_type vec_dot = segment_vec_x * point_vec_x +
                                      segment_vec_y * point_vec_y;
            return vec_dot / sqr_segment_length;
        }

        static int get_intermediate_points_count(const Point2D &point1,
                                                 const Point2D &point2) {
            coordinate_type vec_x = point1.x() - point2.x();
            coordinate_type vec_y = point1.y() - point2.y();
            coordinate_type sqr_len = vec_x * vec_x + vec_y * vec_y;
            return static_cast<int>(log(sqr_len) * 3.4 + 1E-6);
        }
    };
    
    template <typename T>
    class voronoi_cell {
    public:
        typedef T coordinate_type;
        typedef detail::site_event<T> site_event_type;
        typedef voronoi_edge<T> voronoi_edge_type;

        bool contains_segment() const {
            return site_.is_segment();
        }

        const point_2d<T> &get_point0() const {
            return site_.get_point0();
        }

        const point_2d<T> &get_point1() const {
            return site_.get_point1();
        }

        voronoi_edge_type *incident_edge() {
            return incident_edge_;
        }

        const voronoi_edge_type *incident_edge() const {
            return incident_edge_;
        }

        int num_incident_edges() const {
            return num_incident_edges_;
        }

        voronoi_cell(const site_event_type &new_site, voronoi_edge_type *edge) :
            site_(new_site),
            incident_edge_(edge),
            num_incident_edges_(0) {}

    private:
        friend class voronoi_output<T>;
        
        site_event_type site_;
        voronoi_edge_type *incident_edge_;
        int num_incident_edges_;
    };

    template <typename T>
    class voronoi_vertex {
    public:
        typedef T coordinate_type;
        typedef voronoi_edge<T> voronoi_edge_type;

        detail::epsilon_robust_comparator<T> center_x;
        detail::epsilon_robust_comparator<T> center_y;
        detail::epsilon_robust_comparator<T> denom;
        typename std::list< voronoi_vertex<coordinate_type> >::iterator voronoi_record_it;

        voronoi_vertex(const detail::epsilon_robust_comparator<T> &c_x,
                       const detail::epsilon_robust_comparator<T> &c_y,
                       const detail::epsilon_robust_comparator<T> &den,
                       voronoi_edge_type *edge) :
            center_x(c_x),
            center_y(c_y),
            denom(den),
            vertex_(c_x.dif() / den.dif(), c_y.dif() / den.dif()),
            incident_edge_(edge),
            num_incident_edges_(0) {}

        const point_2d<T> &vertex() const {
            return vertex_;
        }

        voronoi_edge_type *incident_edge() {
            return incident_edge_;
        }

        const voronoi_edge_type *incident_edge() const {
            return incident_edge_;
        }

        int num_incident_edges() const {
            return num_incident_edges_;
        }

    private:
        friend class voronoi_output<T>;

        point_2d<T> vertex_;
        voronoi_edge_type *incident_edge_;
        int num_incident_edges_;
    };

    // Half-edge data structure. Represents voronoi edge.
    // Contains: 1) pointer to cell records;
    //           2) pointers to start/end vertices of half-edge;
    //           3) pointers to previous/next half-edges(CCW);
    //           4) pointers to previous/next half-edges rotated
    //              around start point;
    //           5) pointer to twin half-edge.
    template <typename T>
    class voronoi_edge {
    public:
        typedef voronoi_cell<T> voronoi_cell_type;
        typedef voronoi_vertex<T> voronoi_vertex_type;
        typedef voronoi_edge<T> voronoi_edge_type;

        voronoi_edge() :
            cell_(NULL),
            vertex_(NULL),
            twin_(NULL),
            next_(NULL),
            prev_(NULL) {}

        voronoi_cell_type *cell() { return cell_; }
        const voronoi_cell_type *cell() const { return cell_; }

        voronoi_vertex_type *vertex0() { return vertex_; }
        const voronoi_vertex_type *vertex0() const { return vertex_; }

        voronoi_vertex_type *vertex1() { return twin_->vertex0(); }
        const voronoi_vertex_type *vertex1() const { return twin_->vertex0(); }

        voronoi_edge_type *twin() { return twin_; }
        const voronoi_edge_type *twin() const { return twin_; }

        voronoi_edge_type *next() { return next_; }
        const voronoi_edge_type *next() const { return next_; }

        voronoi_edge_type *prev() { return prev_; }
        const voronoi_edge_type *prev() const { return prev_; }

        voronoi_edge_type *rot_next() {
            if (prev_)
                return prev_->twin();
            return NULL;
        }
        const voronoi_edge_type *rot_next() const {
            if (prev_)
                return prev_->twin();
            return NULL;
        }

        voronoi_edge_type *rot_prev() { return twin_->next(); }
        const voronoi_edge_type *rot_prev() const { return twin_->next(); }

    private:
        friend class voronoi_output<T>;

        void cell(voronoi_cell_type *c) { cell_ = c; }
        void vertex0(voronoi_vertex_type *v) { vertex_ = v; }
        void vertex1(voronoi_vertex_type *v) { twin_->vertex0(v); }
        void twin(voronoi_edge_type *e) { twin_ = e; }
        void next(voronoi_edge_type *e) { next_ = e; }
        void prev(voronoi_edge_type *e) { prev_ = e; }

        voronoi_cell_type *cell_;
        voronoi_vertex_type *vertex_;
        voronoi_edge_type *twin_;
        voronoi_edge_type *next_;
        voronoi_edge_type *prev_;
    };

    // Voronoi output data structure based on the half-edges.
    // Contains vector of voronoi cells, doubly linked list of
    // voronoi vertices and voronoi edges.
    template <typename T>
    class voronoi_output {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef detail::site_event<coordinate_type> site_event_type;
        typedef detail::circle_event<coordinate_type> circle_event_type;

        typedef voronoi_cell<coordinate_type> voronoi_cell_type;
        typedef std::vector<voronoi_cell_type> voronoi_cells_type;
        typedef typename voronoi_cells_type::iterator voronoi_cell_iterator_type;
        typedef typename voronoi_cells_type::const_iterator voronoi_cell_const_iterator_type;

        typedef voronoi_vertex<coordinate_type> voronoi_vertex_type;
        typedef std::list<voronoi_vertex_type> voronoi_vertices_type;
        typedef typename voronoi_vertices_type::iterator voronoi_vertex_iterator_type;
        typedef typename voronoi_vertices_type::const_iterator voronoi_vertex_const_iterator_type;

        typedef voronoi_edge<coordinate_type> voronoi_edge_type;
        typedef std::list<voronoi_edge_type> voronoi_edges_type;
        typedef typename voronoi_edges_type::iterator voronoi_edge_iterator_type;
        typedef typename voronoi_edges_type::const_iterator voronoi_edge_const_iterator_type;

        voronoi_output() {
            num_cell_records_ = 0;
            num_edge_records_ = 0;
            num_vertex_records_ = 0;
        }

        void reset() {
            cell_records_.clear();
            vertex_records_.clear();
            edge_records_.clear();

            num_cell_records_ = 0;
            num_edge_records_ = 0;
            num_vertex_records_ = 0;
        }

        const BRect<T> &bounding_rectangle() const {
            return voronoi_rect_;
        }

        const voronoi_cells_type &cell_records() const {
            return cell_records_;
        }

        const voronoi_vertices_type &vertex_records() const {
            return vertex_records_;
        }

        const voronoi_edges_type &edge_records() const {
            return edge_records_;
        }

        int num_cell_records() const {
            return num_cell_records_;
        }

        int num_edge_records() const {
            return num_edge_records_;
        }

        int num_vertex_records() const {
            return num_vertex_records_;
        }

    private:
        friend class detail::voronoi_builder<T>;

        void init(int num_sites) {
            cell_records_.reserve(num_sites);
            num_cell_records_ = 0;
            num_edge_records_ = 0;
            num_vertex_records_ = 0;
        }

        // Update voronoi output in case of single site input.
        void process_single_site(const site_event_type &site) {
            // Update counters.
            num_cell_records_++;

            // Update bounding rectangle.
            voronoi_rect_ = BRect<coordinate_type>(site.get_point0(), site.get_point0());

            // Update cell records.
            cell_records_.push_back(voronoi_cell_type(site, NULL));
        }

        // Inserts new half-edge into the output data structure during site
        // event processing. Takes as input left and right sites of the new
        // beach line node and returns pointer to the new half-edge. 
        voronoi_edge_type *insert_new_edge(const site_event_type &site1,
                                           const site_event_type &site2) {
            // Update counters.
            num_cell_records_++;
            num_edge_records_++;

            // Get indices of sites.           
            int site_index1 = site1.get_site_index();
            int site_index2 = site2.get_site_index();

            // Create new half-edge that belongs to the first site.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &edge1 = edge_records_.back();

            // Create new half-edge that belongs to the second site.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &edge2 = edge_records_.back();

            // Add initial cell during first edge insertion.
            if (cell_records_.empty()) {
                cell_records_.push_back(voronoi_cell_type(site1, &edge1));
                num_cell_records_++;
                voronoi_rect_ = BRect<coordinate_type>(site1.get_point0(), site1.get_point0());
            }
            cell_records_[site_index1].num_incident_edges_++;

            // Update bounding rectangle.
            voronoi_rect_.update(site2.get_point0());
            if (site2.is_segment()) {
                voronoi_rect_.update(site2.get_point1());	
            }

            // Second site represents new site during site event processing.
            // Add new cell to the cell records vector.
            cell_records_.push_back(voronoi_cell_type(site2, &edge2));
            cell_records_.back().num_incident_edges_++;
            
            // Update pointers to cells.
            edge1.cell(&cell_records_[site_index1]);
            edge2.cell(&cell_records_[site_index2]);

            // Update twin pointers.
            edge1.twin(&edge2);
            edge2.twin(&edge1);

            return &edge1;
        }

        voronoi_edge_type *insert_new_edge(const site_event_type &site1,
                                           const site_event_type &site3,
                                           const circle_event_type &circle,
                                           voronoi_edge_type *edge12,
                                           voronoi_edge_type *edge23) {
            // Update counters.
            num_vertex_records_++;
            num_edge_records_++;

            // Update bounding rectangle.
            //voronoi_rect_.update(circle.get_center());

            // Add new voronoi vertex with point at center of the circle.
            vertex_records_.push_back(voronoi_vertex_type(
                circle.get_erc_x(), circle.get_erc_y(), circle.get_erc_denom(), edge12));
            voronoi_vertex_type &new_vertex = vertex_records_.back();
            new_vertex.num_incident_edges_ = 3;
            new_vertex.voronoi_record_it = vertex_records_.end();
            new_vertex.voronoi_record_it--;

            // Update two input bisectors and their twin half-edges with
            // new voronoi vertex.
            edge12->vertex0(&new_vertex);
            edge23->vertex0(&new_vertex);

            // Add new half-edge.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &new_edge1 = edge_records_.back();
            new_edge1.cell(&cell_records_[site1.get_site_index()]);
            new_edge1.cell_->num_incident_edges_++;

            // Add new half-edge.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &new_edge2 = edge_records_.back();
            new_edge2.cell(&cell_records_[site3.get_site_index()]);
            new_edge2.cell_->num_incident_edges_++;

            // Update twin pointers of the new half-edges.
            new_edge1.twin(&new_edge2);
            new_edge2.twin(&new_edge1);
            new_edge2.vertex0(&new_vertex);

            // Update voronoi prev/next pointers of all half-edges incident
            // to the new voronoi vertex.
            edge12->prev(&new_edge1);
            new_edge1.next(edge12);
            edge12->twin_->next(edge23);
            edge23->prev(edge12->twin());
            edge23->twin_->next(&new_edge2);
            new_edge2.prev(edge23->twin());

            return &new_edge1;
        }

        void simplify() {
            voronoi_edge_iterator_type edge_it1;
            voronoi_edge_iterator_type edge_it = edge_records_.begin();

            // Return in case of collinear sites input.
            if (num_vertex_records_ == 0) {
                if (edge_it == edge_records_.end())
                    return;

                voronoi_edge_type *edge1 = &(*edge_it);
                edge1->next(edge1);
                edge1->prev(edge1);
                edge_it++;
                edge1 = &(*edge_it);
                edge_it++;

                while (edge_it != edge_records_.end()) {
                    voronoi_edge_type *edge2 = &(*edge_it);
                    edge_it++;
                
                    edge1->next(edge2);
                    edge1->prev(edge2);
                    edge2->next(edge1);
                    edge2->prev(edge1);

                    edge1 = &(*edge_it);
                    edge_it++;
                }

                edge1->next(edge1);
                edge1->prev(edge1);
                return;
            }

            // Iterate over all edges and remove degeneracies.
            while (edge_it != edge_records_.end()) {
                edge_it1 = edge_it;
                std::advance(edge_it, 2);

                if (!edge_it1->vertex0() || !edge_it1->vertex1())
                    continue;

                const voronoi_vertex_type *p1 = edge_it1->vertex0();
                const voronoi_vertex_type *p2 = edge_it1->vertex1();
                detail::epsilon_robust_comparator<T> lhs1(p1->center_x * p2->denom);
                detail::epsilon_robust_comparator<T> rhs1(p1->denom * p2->center_x);
                detail::epsilon_robust_comparator<T> lhs2(p1->center_y * p2->denom);
                detail::epsilon_robust_comparator<T> rhs2(p1->denom * p2->center_y);

                if (lhs1.compares_undefined(rhs1, 64) && lhs2.compares_undefined(rhs2, 64)) {
                    // Decrease number of cell incident edges.
                    edge_it1->cell_->num_incident_edges_--;
                    edge_it1->twin_->cell_->num_incident_edges_--;

                    // To guarantee N*lon(N) time we merge vertex with
                    // less incident edges to the one with more.
                    if (edge_it1->cell_->incident_edge_ == &(*edge_it1)) {
                        if (edge_it1->cell_->incident_edge_ == edge_it1->next_) {
                            edge_it1->cell_->incident_edge_ = NULL;
                        } else {
                            edge_it1->cell_->incident_edge_ = edge_it1->next_;
                        }
                    }
                    if (edge_it1->twin_->cell_->incident_edge_ == edge_it1->twin_) {
                        if (edge_it1->twin_->cell_->incident_edge_ == edge_it1->twin_->next_) {
                            edge_it1->twin_->cell_->incident_edge_ = NULL;
                        } else {
                            edge_it1->twin_->cell_->incident_edge_ = edge_it1->twin_->next_;
                        }
                    }
                    if (edge_it1->vertex0()->num_incident_edges_ >=
                        edge_it1->vertex1()->num_incident_edges_) {
                            simplify_edge(&(*edge_it1));
                    } else {
                        simplify_edge(edge_it1->twin_);
                    }

                    // Remove zero length edges.
                    edge_records_.erase(edge_it1, edge_it);
                    num_edge_records_--;
                }
            }

            // Make some post processing.
            for (voronoi_cell_iterator_type cell_it = cell_records_.begin();
                cell_it != cell_records_.end(); cell_it++) {
                // Move to the previous edge while it is possible in the CW direction.
                voronoi_edge_type *cur_edge = cell_it->incident_edge_;
                if (cur_edge) {
                    while (cur_edge->prev_ != NULL) {
                        cur_edge = cur_edge->prev_;

                        // Terminate if this is not a boundary cell.
                        if (cur_edge == cell_it->incident_edge_)
                            break;
                    }

                    // Rewind incident edge pointer to the leftmost edge for the boundary cells.
                    cell_it->incident_edge_ = cur_edge;

                    // Set up prev/next half-edge pointers for ray edges.
                    if (cur_edge->prev_ == NULL) {
                        voronoi_edge_type *last_edge = cell_it->incident_edge_;
                        while (last_edge->next_ != NULL)
                            last_edge = last_edge->next_;
                        last_edge->next(cur_edge);
                        cur_edge->prev(last_edge);
                    }
                }
            }
        }

        // Simplify degenerate edge.
        void simplify_edge(voronoi_edge_type *edge) {
            voronoi_vertex_type *vertex1 = edge->vertex0();
            voronoi_vertex_type *vertex2 = edge->vertex1();

            // Update number of incident edges.
            vertex1->num_incident_edges_ += vertex2->num_incident_edges_ - 2;

            // Update second vertex incident edges start and end points.
            voronoi_edge_type *updated_edge = edge->twin()->rot_next();
            while (updated_edge != edge->twin()) {
                updated_edge->vertex0(vertex1);
                updated_edge = updated_edge->rot_next();
            }

            voronoi_edge_type *edge1 = edge;
            voronoi_edge_type *edge2 = edge->twin();

            voronoi_edge_type *edge1_rot_prev = edge1->rot_prev();
            voronoi_edge_type *edge1_rot_next = edge1->rot_next();

            voronoi_edge_type *edge2_rot_prev = edge2->rot_prev();
            voronoi_edge_type *edge2_rot_next = edge2->rot_next();

            // Update prev and next pointers of incident edges.
            edge1_rot_next->twin()->next(edge2_rot_prev);
            edge2_rot_prev->prev(edge1_rot_next->twin());
            edge1_rot_prev->prev(edge2_rot_next->twin());
            edge2_rot_next->twin()->next(edge1_rot_prev);

            // Change incident edge pointer of a vertex if it corresponds to the
            // degenerate edge.
            if (vertex1->incident_edge() == edge)
                vertex1->incident_edge_ = edge->rot_prev();

            // Remove second vertex from the vertex records list.
            if (vertex1->voronoi_record_it != vertex2->voronoi_record_it) {
                vertex_records_.erase(vertex2->voronoi_record_it);
                num_vertex_records_--;
            }
        }

        voronoi_cells_type cell_records_;
        voronoi_vertices_type vertex_records_;
        voronoi_edges_type edge_records_;

        int num_cell_records_;
        int num_vertex_records_;
        int num_edge_records_;

        BRect<coordinate_type> voronoi_rect_;

        // Disallow copy constructor and operator=
        voronoi_output(const voronoi_output&);
        void operator=(const voronoi_output&);
    };

} // sweepline
} // boost

#endif

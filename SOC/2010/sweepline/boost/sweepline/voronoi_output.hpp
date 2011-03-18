// Boost sweepline/voronoi_output.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

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
        class robust_voronoi_vertex;

        template <typename T>
        class voronoi_builder;
    }

    template <typename T>
    class voronoi_cell;

    template <typename T>
    class voronoi_edge;

    template <typename T>
    class voronoi_output;

    // Cartesian 2D point data structure.
    template <typename T>
    class point_2d {
    public:
        typedef T coordinate_type;

        point_2d() {}

        point_2d(coordinate_type x, coordinate_type y) {
            x_ = x;
            y_ = y;
        }

        bool operator==(const point_2d &that) const {
            return (this->x_ == that.x()) && (this->y_ == that.y());
        }

        bool operator!=(const point_2d &that) const {
            return (this->x_ != that.x()) || (this->y_ != that.y());
        }

        // Comparison function.
        // Defines ordering of the points on the 2D plane.
        bool operator<(const point_2d &that) const {
            if (this->x_ != that.x_)
                return this->x_ < that.x_;
            return this->y_ < that.y_;
        }

        bool operator<=(const point_2d &that) const {
            return !(that < (*this));
        }

        bool operator>(const point_2d &that) const {
            return that < (*this);
        }

        bool operator>=(const point_2d &that) const {
            return !((*this) < that);
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
    static inline point_2d<T> make_point_2d(T x, T y) {
        return point_2d<T>(x,y);
    }

    // Bounding rectangle data structure. Contains coordinates
    // of the bottom left and the upper right points of the rectangle.
    template <typename T>
    class BRect {
    public:
        typedef T coordinate_type;
        typedef point_2d<coordinate_type> point_2d_type;

        BRect() {}

        BRect(const point_2d_type &p1, const point_2d_type &p2) {
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

        // Extend the rectangle with a new point.
        void update(const point_2d_type &p) {
            x_min_ = (std::min)(x_min_, p.x());
            y_min_ = (std::min)(y_min_, p.y());
            x_max_ = (std::max)(x_max_, p.x());
            y_max_ = (std::max)(y_max_, p.y());
        }

        // Check whether a point is situated inside the bounding rectangle.
        bool contains(const point_2d_type &p) const {
            return p.x() > x_min_ && p.x() < x_max_ &&
                   p.y() > y_min_ && p.y() < y_max_;
        }

        // Check whether the bounding rectangle has a non-zero area.
        bool is_valid() const {
            return (x_min_ < x_max_) && (y_min_ < y_max_);
        }

        // Return the x-coordinate of the bottom left point of the rectangle.
        coordinate_type x_min() const {
            return x_min_;
        }

        // Return the y-coordinate of the bottom left point of the rectangle.
        coordinate_type y_min() const {
            return y_min_;
        }

        // Return the x-coordinate of the upper right point of the rectangle.
        coordinate_type x_max() const {
            return x_max_;
        }

        // Return the y-coordinate of the upper right point of the rectangle.
        coordinate_type y_max() const {
            return y_max_;
        }

        coordinate_type min_len() const {
            return (std::min)(x_max_ - x_min_, y_max_ - y_min_);
        }

        coordinate_type max_len() const {
            return (std::max)(x_max_ - x_min_, y_max_ - y_min_);
        }

    private:
        coordinate_type x_min_;
        coordinate_type y_min_;
        coordinate_type x_max_;
        coordinate_type y_max_;
    };

    // Voronoi output postprocessing tools.
    template <typename T>
    class voronoi_helper {
    public:
        typedef T coordinate_type;
        typedef point_2d<coordinate_type> point_2d_type;
        typedef BRect<coordinate_type> brect_type;

        // There are three different types of edges:
        //   1) Segment edge - has both endpoints;
        //   2) Ray edge - has one endpoint, infinite
        //                 in the positive direction;
        //   3) Line edge - is infinite in both directions.
        enum kEdgeType {
            SEGMENT = 0,
            RAY = 1,
            LINE = 2,
        };

        // Get a view rectangle based on the voronoi bounding rectangle.
        static BRect<coordinate_type> get_view_rectangle(
                const BRect<coordinate_type> &brect) {
            coordinate_type x_len = (brect.x_max() - brect.x_min());
            coordinate_type y_len = (brect.y_max() - brect.y_min());
            coordinate_type x_mid = (brect.x_max() + brect.x_min()) * 0.5;
            coordinate_type y_mid = (brect.y_max() + brect.y_min()) * 0.5;
            coordinate_type offset = x_len;
            if (offset < y_len)
                offset = y_len;
            if (offset == 0.0)
                offset = 0.5;
            BRect<coordinate_type> new_brect(x_mid - offset, y_mid - offset,
                                             x_mid + offset, y_mid + offset);
            return new_brect;
        }

        // Compute intermediate points for the voronoi edge within the given
        // bounding rectangle. The assumption is made that voronoi rectangle
        // contains all the finite endpoints of the edge.
        // Max_error is the maximum distance (relative to the minor of two
        // rectangle sides) allowed between the parabola and line segments
        // that interpolate it.
        static std::vector<point_2d_type> get_intermediate_points(
                const voronoi_edge<coordinate_type> *edge,
                const BRect<coordinate_type> &brect,
                double max_error) {
            // Retrieve the cell to the left of the current edge.
            const voronoi_cell<coordinate_type> *cell1 = edge->cell();

            // Retrieve the cell to the right of the current edge.
            const voronoi_cell<coordinate_type> *cell2 = edge->twin()->cell();

            // edge_points - contains intermediate points.
            std::vector<point_2d_type> edge_points;
            if (!(cell1->contains_segment() ^ cell2->contains_segment())) {
                // Edge is a segment, ray, or line.
                if (edge->vertex0() != NULL && edge->vertex1() != NULL) {
                    // Edge is a segment. No further processing is required.
                    edge_points.push_back(edge->vertex0()->vertex());
                    edge_points.push_back(edge->vertex1()->vertex());
                } else {
                    // Edge is a ray or line.
                    // Clip it with the bounding rectangle.
                    const point_2d_type &site1 = cell1->point0();
                    const point_2d_type &site2 = cell2->point0();
                    point_2d_type origin((site1.x() + site2.x()) * 0.5,
                                         (site1.y() + site2.y()) * 0.5);
                    point_2d_type direction(site1.y() - site2.y(),
                                            site2.x() - site1.x());

                    // Find intersection points.
                    find_intersections(origin, direction, LINE,
                                       brect, edge_points);

                    // Update endpoints in case edge is a ray.
                    if (edge->vertex1() != NULL)
                        edge_points[1] = edge->vertex1()->vertex();
                    if (edge->vertex0() != NULL)
                        edge_points[0] = edge->vertex0()->vertex();
                }
            } else {
                // point1 - site point;
                const point_2d_type &point1 = (cell1->contains_segment()) ?
                    cell2->point0() : cell1->point0();

                // point2 - startpoint of the segment site;
                const point_2d_type &point2 = (cell1->contains_segment()) ?
                    cell1->point0() : cell2->point0();

                // point3 - endpoint of the segment site;
                const point_2d_type &point3 = (cell1->contains_segment()) ?
                    cell1->point1() : cell2->point1();

                if (edge->vertex0() != NULL && edge->vertex1() != NULL) {
                    // Edge is a segment or parabolic arc.
                    edge_points.push_back(edge->vertex0()->vertex());
                    edge_points.push_back(edge->vertex1()->vertex());
                    double max_dist = max_error * brect.min_len();
                    fill_intermediate_points(point1, point2, point3,
                                             edge_points, max_dist);
                } else {
                    // Edge is a ray or line.
                    // Clip it with the bounding rectangle.
                    coordinate_type dir_x =
                        (cell1->contains_segment() ^ (point1 == point3)) ?
                        point3.y() - point2.y() : point2.y() - point3.y();
                    coordinate_type dir_y =
                        (cell1->contains_segment() ^ (point1 == point3)) ?
                        point2.x() - point3.x() : point3.x() - point2.x();
                    point_2d_type direction(dir_x, dir_y);

                    // Find intersection points.
                    find_intersections(point1, direction, LINE,
                                       brect, edge_points);

                    // Update endpoints in case edge is a ray.
                    if (edge->vertex1() != NULL)
                        edge_points[1] = edge->vertex1()->vertex();
                    if (edge->vertex0() != NULL)
                        edge_points[0] = edge->vertex0()->vertex();
                }
            }
            return edge_points;
        }

        // Find edge-rectangle intersection points.
        // Edge is represented by its origin, direction and type.
        static void find_intersections(
                const point_2d_type &origin, const point_2d_type &direction,
                kEdgeType edge_type, const brect_type &brect,
                std::vector<point_2d_type> &intersections) {
            // Find the center of the rectangle.
            coordinate_type center_x = (brect.x_min() + brect.x_max()) * 0.5;
            coordinate_type center_y = (brect.y_min() + brect.y_max()) * 0.5;

            // Find the half-diagonal vector of the rectangle.
            coordinate_type len_x = brect.x_max() - center_x;
            coordinate_type len_y = brect.y_max() - center_y;

            // Find the vector between the origin and the center of the
            // rectangle.
            coordinate_type diff_x = origin.x() - center_x;
            coordinate_type diff_y = origin.y() - center_y;

            // Find the vector perpendicular to the direction vector.
            coordinate_type perp_x = direction.y();
            coordinate_type perp_y = -direction.x();

            // Projection of the vector between the origin and the center of
            // the rectangle on the line perpendicular to the direction vector.
            coordinate_type lexpr = magnitude(perp_x * diff_x +
                                              perp_y * diff_y);

            // Maximum projection among any of the half-diagonals of the
            // rectangle on the line perpendicular to the direction vector.
            coordinate_type rexpr = magnitude(perp_x * len_x) +
                                    magnitude(perp_y * len_y);

            // Intersection check. Compare projections.
            if (lexpr > rexpr)
                return;

            // Intersection parameters. If fT[i]_used is true than:
            // origin + fT[i] * direction = intersection point, i = 0 .. 1.
            // For different edge types next fT values are acceptable:
            // Segment: [0; 1];
            // Ray: [0; infinity];
            // Line: [-infinity; infinity].
            bool fT0_used = false;
            bool fT1_used = false;
            coordinate_type fT0 = 0;
            coordinate_type fT1 = 0;

            // Check for the intersections with the lines
            // going through the sides of the bounding rectangle.
            clip_line(+direction.x(), -diff_x - len_x,
                      fT0_used, fT1_used, fT0, fT1);
            clip_line(-direction.x(), +diff_x - len_x,
                      fT0_used, fT1_used, fT0, fT1);
            clip_line(+direction.y(), -diff_y - len_y,
                      fT0_used, fT1_used, fT0, fT1);
            clip_line(-direction.y(), +diff_y - len_y,
                      fT0_used, fT1_used, fT0, fT1);

            // Update intersections vector.
            if (fT0_used && check_extent(fT0, edge_type))
                intersections.push_back(make_point_2d(
                    origin.x() + fT0 * direction.x(),
                    origin.y() + fT0 * direction.y()));
            if (fT1_used && fT0 != fT1 && check_extent(fT1, edge_type))
                intersections.push_back(make_point_2d(
                    origin.x() + fT1 * direction.x(),
                    origin.y() + fT1 * direction.y()));
        }

    private:
        voronoi_helper();

        // Find intermediate points of the parabola. Number of points
        // is defined by the value of max_dist parameter - maximum distance
        // between parabola and line segments that interpolate it.
        // Parabola is a locus of points equidistant from the point and segment
        // sites. intermediate_points should contain two initial endpoints
        // of the edge (voronoi vertices). Intermediate points are inserted
        // between the given two endpoints.
        // Max_dist is the maximum distance allowed between parabola and line
        // segments that interpolate it.
        static void fill_intermediate_points(
                point_2d_type point_site,
                point_2d_type segment_site_start,
                point_2d_type segment_site_end,
                std::vector<point_2d_type> &intermediate_points,
                double max_dist) {
            // Check if bisector is a segment.
            if (point_site == segment_site_start ||
                point_site == segment_site_end)
                return;

            // Apply the linear transformation to move start point of the
            // segment to the point with coordinates (0, 0) and the direction
            // of the segment to coincide the positive direction of the x-axis.
            coordinate_type segm_vec_x = segment_site_end.x() -
                                         segment_site_start.x();
            coordinate_type segm_vec_y = segment_site_end.y() -
                                         segment_site_start.y();
            coordinate_type sqr_segment_length = segm_vec_x * segm_vec_x +
                                                 segm_vec_y * segm_vec_y;

            // Compute x-coordinates of the endpoints of the edge
            // in the transformed space.
            coordinate_type projection_start = sqr_segment_length *
                get_point_projection(intermediate_points[0],
                                     segment_site_start,
                                     segment_site_end);
            coordinate_type projection_end = sqr_segment_length *
                get_point_projection(intermediate_points[1],
                                     segment_site_start,
                                     segment_site_end);

            // Compute parabola parameterers in the transformed space.
            // Parabola has next representation:
            // f(x) = ((x-rot_x)^2 + rot_y^2) / (2.0*rot_y).
            coordinate_type point_vec_x = point_site.x() -
                                          segment_site_start.x();
            coordinate_type point_vec_y = point_site.y() -
                                          segment_site_start.y();
            coordinate_type rot_x = segm_vec_x * point_vec_x +
                                    segm_vec_y * point_vec_y;
            coordinate_type rot_y = segm_vec_x * point_vec_y -
                                    segm_vec_y * point_vec_x;

            // Save the last point.
            point_2d_type last_point = intermediate_points[1];
            intermediate_points.pop_back();

            // Use stack to avoid recursion.
            std::stack< coordinate_type > point_stack;
            point_stack.push(projection_end);
            coordinate_type cur_x = projection_start;
            coordinate_type cur_y = parabola_y(cur_x, rot_x, rot_y);

            // Adjust max_dist parameter in the transformed space.
            max_dist *= max_dist * sqr_segment_length;

            while (!point_stack.empty()) {
                coordinate_type new_x = point_stack.top();
                coordinate_type new_y = parabola_y(new_x, rot_x, rot_y);

                // Compute coordinates of the point of the parabola that is
                // furthest from the current line segment.
                coordinate_type mid_x = (new_y - cur_y) / (new_x - cur_x) *
                                        rot_y + rot_x;
                coordinate_type mid_y = parabola_y(mid_x, rot_x, rot_y);

                // Compute maximum distance between the given parabolic arc
                // and line segment that interpolates it.
                double dist = (new_y - cur_y) * (mid_x - cur_x) -
                              (new_x - cur_x) * (mid_y - cur_y);
                dist = dist * dist / ((new_y - cur_y) * (new_y - cur_y) +
                                      (new_x - cur_x) * (new_x - cur_x));
                if (dist <= max_dist) {
                    // Distance between parabola and line segment is
                    // not greater than max_dist.
                    point_stack.pop();
                    coordinate_type inter_x =
                        (segm_vec_x * new_x - segm_vec_y * new_y) /
                        sqr_segment_length + segment_site_start.x();
                    coordinate_type inter_y =
                        (segm_vec_x * new_y + segm_vec_y * new_x) /
                        sqr_segment_length + segment_site_start.y();
                    intermediate_points.push_back(
                        make_point_2d(inter_x, inter_y));
                    cur_x = new_x;
                    cur_y = new_y;
                } else {
                    point_stack.push(mid_x);
                }
            }

            // Update last point.
            intermediate_points.back() = last_point;
        }

        // Compute y(x) = ((x - a) * (x - a) + b * b) / (2 * b).
        static coordinate_type parabola_y(coordinate_type x,
                                          coordinate_type a,
                                          coordinate_type b) {
            return ((x - a) * (x - a) + b * b) / (2.0 * b);
        }

        // Check whether extent is compatible with the edge type.
        static bool check_extent(coordinate_type extent, kEdgeType etype) {
            switch (etype) {
                case SEGMENT:
                    return extent >= static_cast<coordinate_type>(0.0) &&
                           extent <= static_cast<coordinate_type>(1.0);
                case RAY: return extent >= static_cast<coordinate_type>(0.0);
                case LINE: return true;
            }
            return true;
        }

        // Compute the absolute value.
        static inline coordinate_type magnitude(coordinate_type value) {
            if (value >= static_cast<coordinate_type>(0.0))
                return value;
            return -value;
        }

        // Find fT min and max values: fT = numer / denom.
        static void clip_line(coordinate_type denom, coordinate_type numer,
                              bool &fT0_used, bool &fT1_used,
                              coordinate_type &fT0, coordinate_type &fT1) {
            if (denom > static_cast<coordinate_type>(0.0)) {
                if (fT1_used && numer > denom * fT1)
                    return;
                if (!fT0_used || numer > denom * fT0) {
                    fT0_used = true;
                    fT0 = numer / denom;
                }
            } else if (denom < static_cast<coordinate_type>(0.0)) {
                if (fT0_used && numer > denom * fT0)
                    return;
                if (!fT1_used || numer > denom * fT1) {
                    fT1_used = true;
                    fT1 = numer / denom;
                }
            }
        }

        // Get normalized length of the distance between:
        //     1) point projection onto the segment;
        //     2) start point of the segment.
        // Return this length divided by the segment length.
        // This is made to avoid sqrt computation during transformation from
        // the initial space to the transformed one and vice versa.
        // Assumption is made that projection of the point lies
        // between the startpoint and endpoint of the segment.
        static coordinate_type get_point_projection(
                const point_2d_type &point,
                const point_2d_type &segment_start,
                const point_2d_type &segment_end) {
            coordinate_type segment_vec_x = segment_end.x() -
                                            segment_start.x();
            coordinate_type segment_vec_y = segment_end.y() -
                                            segment_start.y();
            coordinate_type point_vec_x = point.x() - segment_start.x();
            coordinate_type point_vec_y = point.y() - segment_start.y();
            coordinate_type sqr_segment_length =
                segment_vec_x * segment_vec_x + segment_vec_y * segment_vec_y;
            coordinate_type vec_dot = segment_vec_x * point_vec_x +
                                      segment_vec_y * point_vec_y;
            return vec_dot / sqr_segment_length;
        }
    };

    // Represents voronoi cell.
    // Data members: 1) pointer to the incident edge;
    //               2) site inside the cell;
    //               3) number of incident edges.
    // The cell may contain point or segment site.
    template <typename T>
    class voronoi_cell {
    public:
        typedef T coordinate_type;
        typedef point_2d<coordinate_type> point_2d_type;
        typedef detail::site_event<coordinate_type> site_event_type;
        typedef voronoi_edge<coordinate_type> voronoi_edge_type;

        voronoi_cell(const point_2d_type &p1, const point_2d_type &p2,
                     bool contains_segment, voronoi_edge_type *edge) :
            point0_(p1),
            point1_(p2),
            contains_segment_(contains_segment),
            incident_edge_(edge),
            num_incident_edges_(0) {}

        // Returns true if the cell contains segment site, false else.
        bool contains_segment() const { return contains_segment_; }

        // Returns site point in case cell contains point site,
        // the first point of the segment site else.
        const point_2d_type &point0() const { return point0_; }

        // Returns the second site of the segment site.
        // Don't use with the point sites.
        const point_2d_type &point1() const { return point1_; }

        voronoi_edge_type *incident_edge() { return incident_edge_; }
        const voronoi_edge_type *incident_edge() const {
            return incident_edge_;
        }

        int num_incident_edges() const { return num_incident_edges_; }

    private:
        friend class voronoi_output<coordinate_type>;

        void incident_edge(voronoi_edge_type *e) { incident_edge_ = e; }
        void inc_num_incident_edges() { ++num_incident_edges_; }
        void dec_num_incident_edges() { --num_incident_edges_; }

        point_2d_type point0_;
        point_2d_type point1_;
        bool contains_segment_;
        voronoi_edge_type *incident_edge_;
        int num_incident_edges_;
    };

    // Represents voronoi vertex.
    // Data members: 1) robust vertex data structure;
    //               2) vertex point itself;
    //               3) pointer to the incident edge;
    //               4) number of incident edges.
    template <typename T>
    class voronoi_vertex {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> point_2d_type;
        typedef voronoi_edge<coordinate_type> voronoi_edge_type;

        voronoi_vertex(const point_2d_type &vertex, voronoi_edge_type *edge) :
            vertex_(vertex),
            incident_edge_(edge),
            num_incident_edges_(3) {}

        const point_2d_type &vertex() const { return vertex_; }

        voronoi_edge_type *incident_edge() { return incident_edge_; }
        const voronoi_edge_type *incident_edge() const {
            return incident_edge_;
        }

        int num_incident_edges() const { return num_incident_edges_; }

    private:
        typedef detail::robust_voronoi_vertex<coordinate_type>
            robust_voronoi_vertex_type;

        friend class voronoi_output<coordinate_type>;

        const robust_voronoi_vertex_type *robust_vertex() const {
            return robust_vertex_;
        }

        void robust_voronoi_vertex(robust_voronoi_vertex_type *v) {
            robust_vertex_ = v;
        }

        void incident_edge(voronoi_edge_type *e) { incident_edge_ = e; }
        void num_incident_edges(int n) { num_incident_edges_ = n; }

        robust_voronoi_vertex_type *robust_vertex_;
        point_2d_type vertex_;
        voronoi_edge_type *incident_edge_;
        int num_incident_edges_;
    };

    // Half-edge data structure. Represents voronoi edge.
    // Variables: 1) pointer to the corresponding cell;
    //            2) pointer to the vertex that is the starting
    //               point of the half-edge;
    //            3) pointer to the twin edge;
    //            4) pointer to the CCW/CW next edge.
    //            5) pointer to the CCW/CW prev edge.
    template <typename T>
    class voronoi_edge {
    public:
        typedef T coordinate_type;
        typedef voronoi_cell<coordinate_type> voronoi_cell_type;
        typedef voronoi_vertex<coordinate_type> voronoi_vertex_type;
        typedef voronoi_edge<coordinate_type> voronoi_edge_type;

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

        // Return a pointer to the rotation next edge
        // over the starting point of the half-edge.
        voronoi_edge_type *rot_next() {
            return (vertex_) ? prev_->twin() : NULL;
        }
        const voronoi_edge_type *rot_next() const {
            return (vertex_) ? prev_->twin() : NULL;
        }

        // Return a pointer to the rotation prev edge
        // over the starting point of the half-edge.
        voronoi_edge_type *rot_prev() {
            return (vertex_) ? twin_->next() : NULL;
        }
        const voronoi_edge_type *rot_prev() const {
            return (vertex_) ? twin_->next() : NULL;
        }

        // Return true if the edge is finite (segment, parabolic arc).
        // Return false if the edge is infinite (ray, line).
        bool is_bounded() const { return vertex0() && vertex1(); }

        // Return true if the edge is linear.
        // Return false if the edge is curved (parabolic arc).
        bool is_linear() const {
            return !(cell()->contains_segment() ^
                     twin()->cell()->contains_segment());
        }

        // Returns true if the edge is curved (parabolic arc).
        // Returns false if the edge is linear.
        bool is_curved() const {
            return (cell()->contains_segment() ^
                    twin()->cell()->contains_segment());
        }

        // Return false if edge goes through the endpoint of the segment.
        // Return true else.
        bool is_primary() const {
            voronoi_cell_type *cell1 = cell_;
            voronoi_cell_type *cell2 = twin_->cell();
            if (cell1->contains_segment() &&
                !cell2->contains_segment()) {
                if (cell1->point0() == cell2->point0() ||
                    cell1->point1() == cell2->point0())
                    return false;
            }
            if (cell2->contains_segment() &&
                !cell1->contains_segment()) {
                if (cell2->point0() == cell1->point0() ||
                    cell2->point1() == cell1->point0())
                    return false;
            }
            return true;
        }

    private:
        friend class voronoi_output<coordinate_type>;

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

    // Voronoi output data structure.
    // Data members:
    //   1) cell_records_ - vector of the voronoi cells;
    //   2) vertex_records_ - list of the voronoi vertices;
    //   3) edge_records_ - list of the voronoi edges;
    //   4) robust_vertices_ - list of the robust vertices;
    //   5) voronoi_rect_ - bounding rectangle;
    //   6) num_cell_records_ - number of the voronoi cells;
    //   7) num_vertex_records_ - number of the voronoi vertices;
    //   8) num_edge_records_ - number of the voronoi edges.
    // CCW ordering is used on the faces perimeter and around the vertices.
    // Robust vertices are used to make the simplification stage epsilon
    // robust. Vector data structure is used to store voronoi cells as the
    // number of the cells may be precomputed at the initialization step.
    // As size() method takes O(n) time on the list data structure three
    // additional counters are used to count the number of the voronoi cells,
    // vertices and edges. As we use list data structure to represent voronoi
    // vertices and edges there is no copy method available, because it will
    // invalidate all the pointers. Another approach could be used to make
    // copying available:
    //     1) use vectors to store voronoi vertices and cells;
    //     2) store vector indices instead of the pointers;
    //     3) store additional pointer to the voronoi output data structure
    //        in the voronoi cell, vertex, edge data structure.
    //     4) implement simplification via copying not degenerate elements
    //        to the new vector as removing elements from a vector takes O(n)
    //        time.
    template <typename T>
    class voronoi_output {
    public:
        typedef T coordinate_type;
        typedef point_2d<coordinate_type> point_2d_type;

        typedef voronoi_cell<coordinate_type> voronoi_cell_type;
        typedef std::vector<voronoi_cell_type> voronoi_cells_type;
        typedef typename voronoi_cells_type::iterator
            voronoi_cell_iterator_type;
        typedef typename voronoi_cells_type::const_iterator
            voronoi_cell_const_iterator_type;

        typedef voronoi_vertex<coordinate_type> voronoi_vertex_type;
        typedef std::list<voronoi_vertex_type> voronoi_vertices_type;
        typedef typename voronoi_vertices_type::iterator
            voronoi_vertex_iterator_type;
        typedef typename voronoi_vertices_type::const_iterator
            voronoi_vertex_const_iterator_type;

        typedef voronoi_edge<coordinate_type> voronoi_edge_type;
        typedef std::list<voronoi_edge_type> voronoi_edges_type;
        typedef typename voronoi_edges_type::iterator
            voronoi_edge_iterator_type;
        typedef typename voronoi_edges_type::const_iterator
            voronoi_edge_const_iterator_type;

        voronoi_output() :
            num_cell_records_(0),
            num_edge_records_(0),
            num_vertex_records_(0) {}

        void clear() {
            robust_vertices_.clear();
            voronoi_cells_type().swap(cell_records_);
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
        typedef detail::site_event<coordinate_type> site_event_type;
        typedef detail::circle_event<coordinate_type> circle_event_type;
        typedef detail::robust_voronoi_vertex<coordinate_type>
            robust_voronoi_vertex_type;

        friend class detail::voronoi_builder<T>;

        void init(int num_sites) {
            // Resize cell vector to avoid reallocations.
            cell_records_.reserve(num_sites);

            // Init counters.
            num_cell_records_ = 0;
            num_edge_records_ = 0;
            num_vertex_records_ = 0;
        }

        // Update the voronoi output in case of a single point input.
        void process_single_site(const site_event_type &site) {
            // Update bounding rectangle.
            voronoi_rect_ = BRect<coordinate_type>(site.point0(),
                                                   site.point0());

            // Update cell records.
            cell_records_.push_back(voronoi_cell_type(site.point0(),
                                                      site.point1(),
                                                      site.is_segment(),
                                                      NULL));
        }

        // Insert a new half-edge into the output data structure.
        // Takes as input left and right sites that form a new bisector.
        // Returns a pointer to a new half-edge.
        voronoi_edge_type *insert_new_edge(const site_event_type &site1,
                                           const site_event_type &site2) {
            // Get sites' indices.
            int site_index1 = site1.index();
            int site_index2 = site2.index();

            // Create a new half-edge that belongs to the first site.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &edge1 = edge_records_.back();

            // Create a new half-edge that belongs to the second site.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &edge2 = edge_records_.back();

            // Add the initial cell during the first edge insertion.
            if (cell_records_.empty()) {
                cell_records_.push_back(voronoi_cell_type(site1.point0(),
                                                          site1.point1(),
                                                          site1.is_segment(),
                                                          &edge1));
                voronoi_rect_ = BRect<coordinate_type>(site1.point0(),
                                                       site1.point0());
            }
            cell_records_[site_index1].inc_num_incident_edges();

            // Update the bounding rectangle.
            voronoi_rect_.update(site2.point0());
            if (site2.is_segment()) {
                voronoi_rect_.update(site2.point1());	
            }

            // The second site represents a new site during site event
            // processing. Add a new cell to the cell records.
            cell_records_.push_back(voronoi_cell_type(site2.point0(),
                                                      site2.point1(),
                                                      site2.is_segment(),
                                                      &edge2));
            cell_records_.back().inc_num_incident_edges();

            // Set up pointers to cells.
            edge1.cell(&cell_records_[site_index1]);
            edge2.cell(&cell_records_[site_index2]);

            // Set up twin pointers.
            edge1.twin(&edge2);
            edge2.twin(&edge1);

            // Return a pointer to the new half-edge.
            return &edge1;
        }

        // Insert a new half-edge into the output data structure with the
        // start at the point where two previously added half-edges intersect.
        // Takes as input two sites that create a new bisector, circle event
        // that correponds to the intersection point of the two old half-edges,
        // pointers to those half-edges. Half-edges' direction goes out of the
        // new voronoi vertex point. Returns a pointer to the new half-edge.
        voronoi_edge_type *insert_new_edge(const site_event_type &site1,
                                           const site_event_type &site3,
                                           const circle_event_type &circle,
                                           voronoi_edge_type *edge12,
                                           voronoi_edge_type *edge23) {
            // Add a new voronoi vertex.
            robust_vertices_.push_back(
                robust_voronoi_vertex_type(circle.erc_x(),
                                           circle.erc_y(),
                                           circle.erc_denom()));
            const robust_voronoi_vertex_type &robust_vertex =
                robust_vertices_.back();
            vertex_records_.push_back(voronoi_vertex_type(
                make_point_2d(robust_vertex.x(), robust_vertex.y()), edge12));
            voronoi_vertex_type &new_vertex = vertex_records_.back();
            new_vertex.robust_voronoi_vertex(&robust_vertices_.back());

            // Update vertex pointers of the old edges.
            edge12->vertex0(&new_vertex);
            edge23->vertex0(&new_vertex);

            // Add a new half-edge.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &new_edge1 = edge_records_.back();
            new_edge1.cell(&cell_records_[site1.index()]);
            new_edge1.cell()->inc_num_incident_edges();

            // Add a new half-edge.
            edge_records_.push_back(voronoi_edge_type());
            voronoi_edge_type &new_edge2 = edge_records_.back();
            new_edge2.cell(&cell_records_[site3.index()]);
            new_edge2.cell()->inc_num_incident_edges();

            // Update twin pointers.
            new_edge1.twin(&new_edge2);
            new_edge2.twin(&new_edge1);

            // Update vertex pointer.
            new_edge2.vertex0(&new_vertex);

            // Update voronoi prev/next pointers.
            edge12->prev(&new_edge1);
            new_edge1.next(edge12);
            edge12->twin()->next(edge23);
            edge23->prev(edge12->twin());
            edge23->twin()->next(&new_edge2);
            new_edge2.prev(edge23->twin());

            // Return a pointer to the new half-edge.
            return &new_edge1;
        }

        // Remove zero-length edges from the voronoi output.
        void simplify() {
            voronoi_edge_iterator_type edge_it1;
            voronoi_edge_iterator_type edge_it = edge_records_.begin();
            num_cell_records_ = cell_records_.size();

            // All the initial sites are colinear.
            if (vertex_records_.empty()) {
                // Update edges counter.
                num_edge_records_ = num_cell_records_ - 1;

                // Return if there are no edges.
                if (edge_it == edge_records_.end())
                    return;

                // Update prev/next pointers of the edges. Those edges won't
                // have any common endpoints, cause they are infinite.
                // But still they follow each other using CCW ordering.
                voronoi_edge_type *edge1 = &(*edge_it);
                edge1->next(edge1);
                edge1->prev(edge1);
                ++edge_it;
                edge1 = &(*edge_it);
                ++edge_it;

                while (edge_it != edge_records_.end()) {
                    voronoi_edge_type *edge2 = &(*edge_it);
                    ++edge_it;

                    edge1->next(edge2);
                    edge1->prev(edge2);
                    edge2->next(edge1);
                    edge2->prev(edge1);

                    edge1 = &(*edge_it);
                    ++edge_it;
                }

                edge1->next(edge1);
                edge1->prev(edge1);
                return;
            }

            // Iterate over all the edges and remove degeneracies
            // (zero-length edges). Edge is considered to be zero-length
            // if both its endpoints lie within some epsilon-rectangle.
            while (edge_it != edge_records_.end()) {
                edge_it1 = edge_it;
                std::advance(edge_it, 2);

                // Degenerate edges exist only among finite edges.
                if (!edge_it1->vertex0() || !edge_it1->vertex1()) {
                    ++num_edge_records_;
                    continue;
                }

                const voronoi_vertex_type *v1 = edge_it1->vertex0();
                const voronoi_vertex_type *v2 = edge_it1->vertex1();

                // Make epsilon robust check.
                if (v1->robust_vertex()->equals(v2->robust_vertex(), 128)) {
                    // Decrease number of cell's incident edges.
                    edge_it1->cell()->dec_num_incident_edges();
                    edge_it1->twin()->cell()->dec_num_incident_edges();

                    // Corresponding cell incident edge pointer
                    // points to the degenerate edge.
                    if (edge_it1->cell()->incident_edge() == &(*edge_it1)) {
                        // Update incident edge pointer.
                        if (edge_it1->cell()->incident_edge() ==
                            edge_it1->next()) {
                            edge_it1->cell()->incident_edge(NULL);
                        } else {
                            edge_it1->cell()->incident_edge(edge_it1->next());
                        }
                    }

                    // Cell corresponding to the twin edge
                    // points to the degenerate edge.
                    if (edge_it1->twin()->cell()->incident_edge() ==
                        edge_it1->twin()) {
                        // Update incident edge pointer.
                        if (edge_it1->twin()->cell()->incident_edge() ==
                            edge_it1->twin()->next()) {
                            edge_it1->twin()->cell()->incident_edge(NULL);
                        } else {
                            edge_it1->twin()->cell()->incident_edge(
                                edge_it1->twin()->next());
                        }
                    }

                    // To guarantee N*lon(N) time we merge vertex with
                    // less incident edges to the one with more.
                    if (v1->num_incident_edges() >= v2->num_incident_edges()) {
                            remove_edge(&(*edge_it1));
                    } else {
                        remove_edge(edge_it1->twin());
                    }

                    // Remove zero-length edge.
                    edge_records_.erase(edge_it1, edge_it);
                } else {
                    // Count not degenerate edge.
                    ++num_edge_records_;
                }
            }
            robust_vertices_.clear();

            // Remove degenerate voronoi vertices with zero incident edges.
            for (voronoi_vertex_iterator_type vertex_it =
                 vertex_records_.begin();
                 vertex_it != vertex_records_.end();) {
                if (vertex_it->incident_edge() == NULL) {
                    vertex_it = vertex_records_.erase(vertex_it);
                } else {
                    ++vertex_it;
                    ++num_vertex_records_;
                }
            }

            // Update prev/next pointers for the ray-edges.
            for (voronoi_cell_iterator_type cell_it = cell_records_.begin();
                 cell_it != cell_records_.end(); ++cell_it) {
                // Move to the previous edge while
                // it is possible in the CW direction.
                voronoi_edge_type *cur_edge = cell_it->incident_edge();
                if (cur_edge) {
                    while (cur_edge->prev() != NULL) {
                        cur_edge = cur_edge->prev();

                        // Terminate if this is not a boundary cell.
                        if (cur_edge == cell_it->incident_edge())
                            break;
                    }

                    // Rewind incident edge pointer to the
                    // CW leftmost edge for the boundary cells.
                    cell_it->incident_edge(cur_edge);

                    // Set up prev/next half-edge pointers for the ray-edges.
                    if (cur_edge->prev() == NULL) {
                        voronoi_edge_type *last_edge =
                            cell_it->incident_edge();
                        while (last_edge->next() != NULL)
                            last_edge = last_edge->next();
                        last_edge->next(cur_edge);
                        cur_edge->prev(last_edge);
                    }
                }
            }
        }

        // Remove degenerate edge.
        void remove_edge(voronoi_edge_type *edge) {
            voronoi_vertex_type *vertex1 = edge->vertex0();
            voronoi_vertex_type *vertex2 = edge->vertex1();

            // Update number of incident edges.
            vertex1->num_incident_edges(vertex1->num_incident_edges() +
                                        vertex2->num_incident_edges() - 2);

            // Update the endpoints of the incident edges to the second vertex.
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

            // Update prev/next pointers for the incident edges.
            edge1_rot_next->twin()->next(edge2_rot_prev);
            edge2_rot_prev->prev(edge1_rot_next->twin());
            edge1_rot_prev->prev(edge2_rot_next->twin());
            edge2_rot_next->twin()->next(edge1_rot_prev);

            // Change the pointer to the incident edge if it points to the
            // degenerate edge.
            if (vertex1->incident_edge() == edge) {
                vertex1->incident_edge(edge->rot_prev());
            }

            // Set the incident edge point of the second vertex to NULL value.
            if (vertex1 != vertex2) {
                vertex2->incident_edge(NULL);
            }
        }

        std::list< robust_voronoi_vertex_type > robust_vertices_;
        voronoi_cells_type cell_records_;
        voronoi_vertices_type vertex_records_;
        voronoi_edges_type edge_records_;

        int num_cell_records_;
        int num_edge_records_;
        int num_vertex_records_;

        BRect<coordinate_type> voronoi_rect_;

        // Disallow copy constructor and operator=
        voronoi_output(const voronoi_output&);
        void operator=(const voronoi_output&);
    };

} // sweepline
} // boost

#endif

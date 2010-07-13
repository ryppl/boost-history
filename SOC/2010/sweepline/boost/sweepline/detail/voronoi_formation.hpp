// Boost sweepline/voronoi_formation.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_FORMATION
#define BOOST_SWEEPLINE_VORONOI_FORMATION

#include <list>
#include <map>
#include <queue>
#include <vector>

namespace boost {
namespace sweepline {
namespace detail {

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI EVENT TYPES ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct beach_line_node;

    template <typename T>
    struct half_edge;

    template <typename BeachLineNode>
    struct node_comparer;

    // Site event type. 
    // Occurs when sweepline sweeps over one of the initial sites.
    // Contains index of a site below the other sorted sites.
    template <typename T>
    struct site_event {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;

        site_event() {}
        
        site_event(coordinate_type x, coordinate_type y, int index) :
            point_(x, y), site_index_(index) {}

        site_event(const Point2D &point, int index) :
            point_(point), site_index_(index) {}

        bool operator==(const site_event &s_event) const {
            return point_ == s_event.get_point();
        }

        bool operator!=(const site_event &s_event) const {
            return point_ != s_event.get_point();
        }

        bool operator<(const site_event &s_event) const {
            return point_ < s_event.get_point();
        }

        bool operator<=(const site_event &s_event) const {
            return point_ <= s_event.get_point();
        }

        bool operator>(const site_event &s_event) const {
            return point_ > s_event.get_point();
        }

        bool operator>=(const site_event &s_event) const {
            return point_ >= s_event.get_point();
        }

        coordinate_type x() const {
            return point_.x();
        }

        coordinate_type y() const {
            return point_.y();
        }

        const Point2D &get_point() const {
            return point_;
        }

        int get_site_index() const {
            return site_index_;
        }

    private:
        Point2D point_;
        int site_index_;
    };

    template <typename T>
    site_event<T> make_site_event(T x, T y, int index) {
        return site_event<T>(x, y, index);
    }

    template <typename T>
    site_event<T> make_site_event(const point_2d<T> &point, int index) {
        return site_event<T>(point, index);
    }

    // Circle event type. Occurs when sweepline sweeps over the bottom point of
    // the voronoi circle (with center at the bisectors intersection point).
    // Circle event contains circle center and squared radius (to avoid sqrt
    // computation). To compare bottom points of two different voronoi circles
    // we don't compute exact radius and use special arithmetic for that. This 
    // way voronoi diagram implementation may be used with rational arithmetic.
    // Let circle center coordinates be (x, y), squared radius be r. 
    // Bottom point of the voronoi circle will be defined as (x + sqrt(r), y).
    // Contains reference to the second bisector node (ordered from left to
    // right in the beach line) that creates given circle event.
    template <typename T>
    struct circle_event {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef beach_line_node<coordinate_type> Key;
        typedef half_edge<coordinate_type>* Value;
        typedef node_comparer<Key> NodeComparer;
        typedef typename std::map< Key, Value, NodeComparer >::iterator beach_line_iterator;

        circle_event() {}

        circle_event(coordinate_type c_x, coordinate_type c_y, coordinate_type sqr_r) :
            center_(c_x, c_y), sqr_radius_(sqr_r) {}

        circle_event(const Point2D &center, coordinate_type sqr_r) :
            center_(center), sqr_radius_(sqr_r) {}

        circle_event(const circle_event& c_event) {
            center_ = c_event.center_;
            sqr_radius_ = c_event.sqr_radius_;
            bisector_node_ = c_event.bisector_node_;
            for (int i = 0; i < 3; i++)
                sites_[i] = c_event.sites_[i];
        }

        void operator=(const circle_event& c_event) {
            center_ = c_event.center_;
            sqr_radius_ = c_event.sqr_radius_;
            bisector_node_ = c_event.bisector_node_;
            for (int i = 0; i < 3; i++)
                sites_[i] = c_event.sites_[i];
        }

        bool equals(const circle_event &c_event) const {
            return center_.x() == c_event.x() && center_.y() == c_event.y() &&
                   sqr_radius_ == c_event.sqr_radius_ &&
                   sites_[0] == c_event.sites_[0] &&
                   sites_[1] == c_event.sites_[1] &&
                   sites_[2] == c_event.sites_[2];
        }

        bool operator==(const circle_event &c_event) const {
            if (sites_[0] != c_event.sites_[0] ||
                sites_[1] != c_event.sites_[1] ||
                sites_[2] != c_event.sites_[2])
                return false;

            if (center_.y() != c_event.y())
                return false;

            coordinate_type sqr_dif_x = (center_.x() - c_event.x()) *
                                        (center_.x() - c_event.x());
            coordinate_type sum_r_sqr = sqr_radius_ + c_event.sqr_radius_;
            coordinate_type value_left = (sum_r_sqr - sqr_dif_x) * (sum_r_sqr - sqr_dif_x);
            coordinate_type value_right = static_cast<coordinate_type>(4) * sqr_radius_ *
                            c_event.sqr_radius_;

            return value_left == value_right;
        }

        bool operator!=(const circle_event &c_event) const {
            return !((*this) == c_event);
        }

        bool operator<(const circle_event &c_event) const {
            coordinate_type x1 = center_.x();
            coordinate_type y1 = center_.y();
            coordinate_type sqr_r1 = sqr_radius_;
            coordinate_type x2 = c_event.x();
            coordinate_type y2 = c_event.y();
            coordinate_type sqr_r2 = c_event.get_sqr_radius();

            coordinate_type sqr_dif_x = (x1 - x2) * (x1 - x2);
            coordinate_type sum_r_sqr = sqr_r1 + sqr_r2;
            coordinate_type value_left = (sum_r_sqr - sqr_dif_x) * (sum_r_sqr - sqr_dif_x);
            coordinate_type value_right = static_cast<coordinate_type>(4) * sqr_r1 * sqr_r2;
            
            if (x1 > x2) {
                if (sqr_r2 <= sqr_r1)
                    return false;
                
                if (sqr_dif_x > sum_r_sqr)
                    return false;

                if (value_left != value_right)
                    return value_left > value_right;
                
                if (y1 != y2)
                    return y1 < y2;

                if (sites_[0] != c_event.sites_[0])
                    return sites_[0] < c_event.sites_[0];

                if (sites_[1] != c_event.sites_[1])
                    return sites_[1] < c_event.sites_[1];

                return sites_[2] < c_event.sites_[2];
            }
            else if (x1 < x2) {
                if (sqr_r1 <= sqr_r2)
                    return true;

                if (sqr_dif_x > sum_r_sqr)
                    return true;

                if (value_left != value_right)
                    return value_left < value_right;

                if (y1 != y2)
                    return y1 < y2;
                
                if (sites_[0] != c_event.sites_[0])
                    return sites_[0] < c_event.sites_[0];

                if (sites_[1] != c_event.sites_[1])
                    return sites_[1] < c_event.sites_[1];

                return sites_[2] < c_event.sites_[2];
            }
            else {
                if (sqr_r1 != sqr_r2)
                    return sqr_r1 < sqr_r2;

                if (y1 != y2)
                    return y1 < y2;

                if (sites_[0] != c_event.sites_[0])
                    return sites_[0] < c_event.sites_[0];

                if (sites_[1] != c_event.sites_[1])
                    return sites_[1] < c_event.sites_[1];

                return sites_[2] < c_event.sites_[2];
            }
        }

        bool operator<=(const circle_event &c_event) const {
            return !(c_event < (*this));
        }

        bool operator>(const circle_event &c_event) const {
            return c_event < (*this);
        }

        bool operator>=(const circle_event &c_event) const {
            return !((*this) < c_event);
        }

        // Compares bottom voronoi circle point with site event point.
        // If circle point is less then site point return -1.
        // If circle point is equal to site point return 0.
        // If circle point is greater then site point return 1.
        int compare(const site_event<coordinate_type> &s_event) const {
            if (s_event.x() < center_.x())
                return 1;
            coordinate_type sqr_dif_x = (s_event.x() - center_.x()) * (s_event.x() - center_.x());
            if (sqr_dif_x == sqr_radius_) {
                if (center_.y() == s_event.y())
                    return 0;
                return (center_.y() < s_event.y()) ? -1 : 1;
            }
            return (sqr_dif_x < sqr_radius_) ? 1 : -1;
        }

        coordinate_type x() const {
            return center_.x();
        }

        coordinate_type y() const {
            return center_.y();
        }

        const Point2D &get_center() const {
            return center_;
        }

        const coordinate_type &get_sqr_radius() const {
            return sqr_radius_;
        }

        void set_bisector(beach_line_iterator iterator) {
            bisector_node_ = iterator;
        }

        void set_sites(const site_event<coordinate_type> &site1,
                       const site_event<coordinate_type> &site2,
                       const site_event<coordinate_type> &site3) {
            sites_[0] = site1;
            sites_[1] = site2;
            sites_[2] = site3;
        }

        const beach_line_iterator &get_bisector() const {
            return bisector_node_;
        }

        const site_event<coordinate_type>* get_sites() const {
            return sites_;
        }

    private:
        Point2D center_;
        coordinate_type sqr_radius_;
        beach_line_iterator bisector_node_;
        site_event<coordinate_type> sites_[3];
    };

    template <typename T>
    circle_event<T> make_circle_event(T c_x, T c_y, T sqr_radius) {
        return circle_event<T>(c_x, c_y, sqr_radius);
    }

    template <typename T>
    circle_event<T> make_circle_event(point_2d<T> center, T sqr_radius) {
        return circle_event<T>(center, sqr_radius);
    }

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI CIRCLE EVENTS QUEUE ////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    // Event queue data structure, processes circle events.
    template <typename T>
    class circle_events_queue {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef circle_event<T> circle_event_type;

        circle_events_queue() {}

        void reset() {
            while (!circle_events_.empty())
                circle_events_.pop();
            while (!deactivated_events_.empty())
                deactivated_events_.pop();
        }

        bool empty() {
            remove_not_active_events();
            if (!circle_events_.empty())
                return false;
            return true;
        }

        const circle_event_type &top() {
            remove_not_active_events();
            return circle_events_.top();
        }

        void pop() {
            circle_events_.pop();
        }

        void push(const circle_event_type &c_event) {
            circle_events_.push(c_event);
        }

        void deactivate_event(const circle_event_type &c_event) {
            deactivated_events_.push(c_event);
        }

    private:
        void remove_not_active_events() {
            while (!circle_events_.empty() && !deactivated_events_.empty() &&
                   circle_events_.top().equals(deactivated_events_.top())) {
                circle_events_.pop();
                deactivated_events_.pop();
            }
        }

        std::priority_queue< circle_event_type,
                             std::vector<circle_event_type>,
                             std::greater<circle_event_type> > circle_events_;
        std::priority_queue< circle_event_type,
                             std::vector<circle_event_type>,
                             std::greater<circle_event_type> > deactivated_events_;

        //Disallow copy constructor and operator=
        circle_events_queue(const circle_events_queue&);
        void operator=(const circle_events_queue&);
    };

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI BEACH LINE TYPES ///////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    // Represents bisector made by two arcs that correspond to the left and
    // right sites. Arc is defined as curve with points equidistant from the
    // site and from the sweepline.
    // Let sweepline sweep from left to right and it's current coordinate
    // be x0, site coordinates be (x1, y1). In this case equation of the arc
    // may be written as: (x-x0)^2 = (x-x1)^2 + (y-y1)^2, or
    // x = ((y - y1)^2 + x1^2 - x0^2) / (2*(x1 - x0)).
    // In general case two arcs will create two different bisectors. That's why
    // the order of arcs is important to define unique bisector.
    template <typename T>
    struct beach_line_node {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef site_event<T> site_event_type;

        beach_line_node() {}

        // Constructs degenerate bisector, used to search arc that is above
        // given site. The input to the constructor is the site event point.
        explicit beach_line_node(const site_event_type &new_point) {
            left_site_ = new_point;
            right_site_ = new_point;
        }

        // Constructs new bisector. The input to the constructor is two sites
        // that create bisector. The order of sites is important.
        beach_line_node(const site_event_type &left_point,
                        const site_event_type &right_point) {
            left_site_ = left_point;
            right_site_ = right_point;
        }

        // Returns the left site of the bisector.
        const site_event_type &get_left_site() const {
            return left_site_;
        }

        // Returns  the right site of the bisector.
        const site_event_type &get_right_site() const {
            return right_site_;
        }

        void set_left_site(const site_event_type &site) {
            left_site_ = site;
        }

        void set_right_site(const site_event_type &site) {
            right_site_ = site;
        }

        // Returns x coordinate of the rightmost site.
        coordinate_type get_sweepline_coord() const {
            return (std::max)(left_site_.x(), right_site_.x());
        }

        // Returns the rightmost site.
        const site_event_type& get_new_site() const {
            if (left_site_.x() > right_site_.x())
                return left_site_;
            return right_site_;
        }

        // Returns true if horizontal line going through new site intersects
        // right arc at first, else returns false. If horizontal line goes
        // through intersection point of the given two arcs returns false also. 
        // Used during nodes comparison.
        // Let x0 be sweepline coordinate, left site coordinates be (x1, y1),
        // right site coordinates be (x2, y2). Equations of the arcs will be:
        // x1(y) = ((y - y1)^2 + x1^2 - x0^2) / (2*(x1 - x0));
        // x2(y) = ((y - y2)^2 + x2^2 - x0^2) / (2*(x2 - x0)).
        // Horizontal line going throught site (x*, y*) intersects second arc
        // at first if x2(y*) > x1(y*) or:
        // (x2-x0)*(x1-x0)*(x1-x2) + (x2-x0)*(y*-y1)^2 < (x1-x0)*(y*-y2)^2
        bool less(const site_event_type &new_site) const {
            coordinate_type sweepline_coord = new_site.x();
            coordinate_type new_node_coord = new_site.y();
            coordinate_type a1 = left_site_.x() - sweepline_coord;
            coordinate_type a2 = right_site_.x() - sweepline_coord;
            coordinate_type b1 = new_node_coord - left_site_.y();
            coordinate_type b2 = new_node_coord - right_site_.y();
            coordinate_type c = left_site_.x() - right_site_.x();
            return a1 * a2 * c + a2 * b1 * b1 < a1 * b2 * b2;
        }

    private:
        site_event_type left_site_;
        site_event_type right_site_;
    };

    template <typename BeachLineNode>
    struct node_comparer {
    public:
        typedef typename BeachLineNode::coordinate_type coordinate_type;

        // Compares nodes in the balanced binary search tree. Nodes are
        // compared based on the y coordinates of the arcs intersection points.
        // Nodes with lesser y coordinate of the intersection point go first.
        // Comparison is only called during site events processing. That's why
        // one of the nodes will always lie on the sweepline. Comparison won't
        // work fine for nodes situated above sweepline.
        bool operator() (const BeachLineNode &node1,
                         const BeachLineNode &node2) const {
            // Get x coordinate of the righmost site from both nodes.
            coordinate_type node1_line = node1.get_sweepline_coord();
            coordinate_type node2_line = node2.get_sweepline_coord();

            if (node1_line < node2_line) {
                coordinate_type left_site_x = node1.get_left_site().x();
                coordinate_type left_site_y = node1.get_left_site().y();
                coordinate_type right_site_x = node1.get_right_site().x();
                coordinate_type right_site_y = node1.get_right_site().y();
                coordinate_type new_node_y = node2.get_new_site().y();
                if (left_site_x > right_site_x) {
                    if (new_node_y <= left_site_y)
                        return false;
                    return node1.less(node2.get_new_site());
                } else if (left_site_x < right_site_x) {
                    if (new_node_y >= right_site_y)
                        return true;
                    return node1.less(node2.get_new_site());
                } else {
                    return left_site_y + right_site_y <
                        static_cast<coordinate_type>(2.0) * new_node_y;
                }
            } else if (node1_line > node2_line) {
                coordinate_type left_site_x = node2.get_left_site().x();
                coordinate_type left_site_y = node2.get_left_site().y();
                coordinate_type right_site_x = node2.get_right_site().x();
                coordinate_type right_site_y = node2.get_right_site().y();
                coordinate_type new_node_y = node1.get_new_site().y();
                if (left_site_x > right_site_x) {
                    if (new_node_y <= left_site_y)
                        return true;
                    return !node2.less(node1.get_new_site());
                } else if (left_site_x < right_site_x) {
                    if (new_node_y >= right_site_y)
                        return false;
                    return !node2.less(node1.get_new_site());
                } else {
                    return !(left_site_y + right_site_y <
                        static_cast<coordinate_type>(2.0) * new_node_y);
                }
            } else {
                // Both nodes are situated on the same vertical line.
                // Let A be the new site event point, and B the site that
                // creates arc above A. In this case two new nodes are being
                // inserted: (A,B) and (B,A). As intersection points for the
                // first node and for the second are the same we need to
                // compare them based on some another characteristic.
                // That's why we assume that node (C, D) goes before node
                // (D, C), only if D lies on the sweepline.
                if (node1.get_left_site().get_site_index() ==
                    node2.get_right_site().get_site_index() &&
                    node1.get_right_site().get_site_index() ==
                    node2.get_left_site().get_site_index())
                    return node1.get_right_site().x() == node1_line;

                // Just compare coordinates of the sites situated on the sweepline.
                return node1.get_new_site().y() < node2.get_new_site().y();
            }
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT /////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////    
    // Voronoi record data structure. May represent voronoi cell or
    // voronoi vertex. Contains pointer to any incident edge, point
    // coordinates and number of incident edges.
    template <typename T>
    struct voronoi_record {
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        
        half_edge<coordinate_type> *incident_edge;
        Point2D voronoi_point;
        int num_incident_edges;
        typename std::list< voronoi_record<coordinate_type> >::iterator voronoi_record_it;

        voronoi_record(const Point2D &point, half_edge<coordinate_type>* edge) :
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
    //           5) pointer to twin half-edge;
    //           6) pointer to clipped edge during clipping.
    template <typename T>
    struct half_edge {
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef voronoi_record<coordinate_type> voronoi_record_type;
        typedef half_edge<coordinate_type> half_edge_type;
        typedef half_edge_clipped<coordinate_type> half_edge_clipped_type;

        voronoi_record_type *cell;
        voronoi_record_type *start_point;
        voronoi_record_type *end_point;
        half_edge_type *prev;
        half_edge_type *next;
        half_edge_type *rot_prev;
        half_edge_type *rot_next;
        half_edge_type *twin;
        half_edge_clipped_type *clipped_edge;

        half_edge() :
            cell(NULL),
            start_point(NULL),
            end_point(NULL),
            prev(NULL),
            next(NULL),
            rot_prev(NULL),
            rot_next(NULL),
            twin(NULL),
            clipped_edge(NULL) {}
    };

    // Voronoi output data structure based on the half-edges.
    // Contains vector of voronoi cells, doubly linked list of
    // voronoi vertices and voronoi edges.
    template <typename T>
    class voronoi_output {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef typename detail::site_event<coordinate_type> site_event_type;
        typedef typename detail::circle_event<coordinate_type> circle_event_type;

        typedef voronoi_record<coordinate_type> voronoi_record_type;
        typedef voronoi_record_clipped<coordinate_type> clipped_voronoi_record_type;
        typedef std::list<voronoi_record_type> voronoi_records_type;
        typedef typename voronoi_records_type::iterator voronoi_iterator_type;
        typedef typename voronoi_records_type::const_iterator voronoi_const_iterator_type;

        typedef half_edge<coordinate_type> edge_type;
        typedef half_edge_clipped<coordinate_type> clipped_edge_type;
        typedef std::list<edge_type> voronoi_edges_type;
        typedef typename voronoi_edges_type::iterator edges_iterator_type;


        enum kEdgeType {
            SEGMENT = 0,
            RAY = 1,
            LINE = 2,
        };

        voronoi_output() {
            num_cell_records_ = 0;
            num_edges_ = 0;
            num_vertex_records_ = 0;
        }

        void init(int num_sites) {
            cell_iterators_.reserve(num_sites);
            num_cell_records_ = 0;
            num_edges_ = 0;
            num_vertex_records_ = 0;
        }

        void reset() {
            cell_iterators_.clear();
            cell_records_.clear();
            vertex_records_.clear();
            edges_.clear();

            num_cell_records_ = 0;
            num_edges_ = 0;
            num_vertex_records_ = 0;
        }

        // Update voronoi output in case of single site input.
        void process_single_site(const site_event_type &site) {
            // Update counters.
            num_cell_records_++;

            // Update bounding rectangle.
            voronoi_rect_ = BRect<coordinate_type>(site.get_point(), site.get_point());

            // Update cell records.
            cell_records_.push_back(voronoi_record_type(site.get_point(), NULL));
        }

        // Inserts new half-edge into the output data structure during site
        // event processing. Takes as input left and right sites of the new
        // beach line node and returns pointer to the new half-edge. 
        edge_type *insert_new_edge(const site_event_type &site1,
                                   const site_event_type &site2) {
            // Update counters.
            num_cell_records_++;
            num_edges_++;

            // Get indices of sites.           
            int site_index1 = site1.get_site_index();
            int site_index2 = site2.get_site_index();

            // Create new half-edge that belongs to the first site.
            edges_.push_back(edge_type());
            edge_type &edge1 = edges_.back();

            // Create new half-edge that belongs to the second site.
            edges_.push_back(edge_type());
            edge_type &edge2 = edges_.back();

            // Add initial cell during first edge insertion.
            if (cell_records_.empty()) {
                cell_iterators_.push_back(cell_records_.insert(
                    cell_records_.end(), voronoi_record_type(site1.get_point(), &edge1)));
                cell_records_.back().num_incident_edges++;
                num_cell_records_++;
                voronoi_rect_ = BRect<coordinate_type>(site1.get_point(), site1.get_point());
            }

            // Update bounding rectangle.
            voronoi_rect_.update(site2.get_point());

            // Second site represents new site during site event processing.
            // Add new cell to the cell records vector.
            cell_iterators_.push_back(cell_records_.insert(
                cell_records_.end(), voronoi_record_type(site2.get_point(), &edge2)));
            cell_records_.back().num_incident_edges++;
            
            // Update pointers to cells.
            edge1.cell = &(*cell_iterators_[site_index1]);
            edge2.cell = &(*cell_iterators_[site_index2]);

            // Update twin pointers.
            edge1.twin = &edge2;
            edge2.twin = &edge1;

            return &edge1;
        }

        edge_type *insert_new_edge(const site_event_type &site1,
                                   const site_event_type &site2,
                                   const site_event_type &site3,
                                   const circle_event_type &circle,
                                   edge_type *edge12,
                                   edge_type *edge23) {
            // Update counters.
            num_vertex_records_++;
            num_edges_++;

            // Update bounding rectangle.
            voronoi_rect_.update(circle.get_center());

            // Add new voronoi vertex with point at center of the circle.
            vertex_records_.push_back(voronoi_record_type(circle.get_center(), edge12));
            voronoi_record_type &new_vertex = vertex_records_.back();
            new_vertex.num_incident_edges = 3;
            new_vertex.voronoi_record_it = vertex_records_.end();
            new_vertex.voronoi_record_it--;

            // Update two input bisectors and their twin half-edges with
            // new voronoi vertex.
            edge12->start_point = &new_vertex;
            edge12->twin->end_point = &new_vertex;
            edge23->start_point = &new_vertex;
            edge23->twin->end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type());
            edge_type &new_edge1 = edges_.back();
            new_edge1.cell = &(*cell_iterators_[site1.get_site_index()]);
            new_edge1.cell->num_incident_edges++;
            new_edge1.end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type());
            edge_type &new_edge2 = edges_.back();
            new_edge2.cell = &(*cell_iterators_[site3.get_site_index()]);
            new_edge2.cell->num_incident_edges++;
            new_edge2.start_point = &new_vertex;

            // Update twin pointers of the new half-edges.
            new_edge1.twin = &new_edge2;
            new_edge2.twin = &new_edge1;

            // Update voronoi prev/next pointers of all half-edges incident
            // to the new voronoi vertex.
            edge12->prev = &new_edge1;
            new_edge1.next = edge12;
            edge12->twin->next = edge23;
            edge23->prev = edge12->twin;
            edge23->twin->next = &new_edge2;
            new_edge2.prev = edge23->twin;

            // Update voronoi vertices incident edges pointers.
            edge12->rot_next = &new_edge2;
            new_edge2.rot_prev = edge12;
            edge23->rot_next = edge12;
            edge12->rot_prev = edge23;
            new_edge2.rot_next = edge23;
            edge23->rot_prev = &new_edge2;

            return &new_edge1;
        }

        const voronoi_records_type &get_cell_records() const {
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
            return num_edges_;
        }

        const BRect<coordinate_type> &get_bounding_rectangle() const {
            return voronoi_rect_;
        }

        void simplify() {
            edges_iterator_type edge_it1;
            edges_iterator_type edge_it = edges_.begin();

            // Return in case of collinear sites input.
            if (num_vertex_records_ == 0) {
                if (edge_it == edges_.end())
                    return;

                edge_type *edge1 = &(*edge_it);
                edge1->next = edge1->prev = edge1;
                edge_it++;
                edge1 = &(*edge_it);
                edge_it++;

                while (edge_it != edges_.end()) {
                    edge_type *edge2 = &(*edge_it);
                    edge_it++;
                
                    edge1->next = edge1->prev = edge2;
                    edge2->next = edge2->prev = edge1;

                    edge1 = &(*edge_it);
                    edge_it++;
                }

                edge1->next = edge1->prev = edge1;
                return;
            }

            // Iterate over all edges and remove degeneracies.
            while (edge_it != edges_.end()) {
                edge_it1 = edge_it;
                edge_it++;
                edge_it++;

                if (edge_it1->start_point && edge_it1->end_point &&
                    edge_it1->start_point->voronoi_point ==
                    edge_it1->end_point->voronoi_point) {
                    // Decrease number of cell incident edges.
                    edge_it1->cell->num_incident_edges--;
                    edge_it1->twin->cell->num_incident_edges--;

                    // To guarantee N*lon(N) time we merge vertex with
                    // less incident edges to the one with more.
                    if (edge_it1->start_point->num_incident_edges >=
                        edge_it1->end_point->num_incident_edges)
                            simplify_edge(&(*edge_it1));
                    else
                        simplify_edge(&(*edge_it1->twin));

                    // Remove zero length edges.
                    edges_.erase(edge_it1, edge_it);

                    // Update counters.
                    num_vertex_records_--;
                    num_edges_--;
                }
            }

            // Make some post processing.
            for (voronoi_iterator_type cell_it = cell_records_.begin();
                cell_it != cell_records_.end(); cell_it++) {
                // Move to the previous edge while it is possible in the CW direction.
                edge_type *cur_edge = cell_it->incident_edge;
                while (cur_edge->prev != NULL) {
                    cur_edge = cur_edge->prev;

                    // Terminate if this is not a boundary cell.
                    if (cur_edge == cell_it->incident_edge)
                        break;
                }

                // Rewind incident edge pointer to the leftmost edge for the boundary cells.
                cell_it->incident_edge = cur_edge;

                // Set up prev/next half-edge pointers for ray edges.
                if (cur_edge->prev == NULL) {
                    edge_type *last_edge = cell_it->incident_edge;
                    while (last_edge->next != NULL)
                        last_edge = last_edge->next;
                    last_edge->next = cur_edge;
                    cur_edge->prev = last_edge;
                }
            }
            
        }

        void clip(voronoi_output_clipped<coordinate_type> &clipped_output) {
            coordinate_type x_len = (voronoi_rect_.x_max - voronoi_rect_.x_min);
            coordinate_type y_len = (voronoi_rect_.y_max - voronoi_rect_.y_min);
            coordinate_type x_mid = (voronoi_rect_.x_max + voronoi_rect_.x_min) /
                static_cast<coordinate_type>(2);
            coordinate_type y_mid = (voronoi_rect_.y_max + voronoi_rect_.y_min) /
                static_cast<coordinate_type>(2);

            coordinate_type offset = x_len;
            if (offset < y_len)
                offset = y_len;
            offset *= static_cast<coordinate_type>(0.55);

            if (offset == static_cast<coordinate_type>(0))
                offset = 0.5;

            BRect<coordinate_type> new_brect(x_mid - offset, y_mid - offset,
                                     x_mid + offset, y_mid + offset);
            clip(new_brect, clipped_output);
        }

        void clip(const BRect<coordinate_type> &brect,
                  voronoi_output_clipped<coordinate_type> &clipped_output) {
            if (!brect.valid())
                return;
            clipped_output.reset();
            clipped_output.set_bounding_rectangle(brect);
            
            // collinear input sites case.
            if (num_vertex_records_ == 0) {
                // Return in case of single site input.
                if (num_cell_records_ == 1) {
                    clipped_output.add_cell(cell_records_.begin()->voronoi_point);
                    return;
                }

                edges_iterator_type edge_it = edges_.begin();
                while (edge_it != edges_.end()) {
                    edge_type *cur_edge = &(*edge_it);
                    edge_it++;
                    edge_type *cur_twin_edge = &(*edge_it);
                    edge_it++;

                    std::vector<Point2D> intersections;
                    const Point2D &site1 = cur_edge->cell->voronoi_point;
                    const Point2D &site2 = cur_twin_edge->cell->voronoi_point;
                    Point2D origin((site1.x() + site2.x()) * static_cast<coordinate_type>(0.5),
                                   (site1.y() + site2.y()) * static_cast<coordinate_type>(0.5));
                    Point2D direction(site1.y() - site2.y(), site2.x() - site1.x());
                    find_intersections(origin, direction, LINE, brect, intersections);
                    if (intersections.size() == 2) {
                        // Add new clipped edges.
                        clipped_edge_type &new_edge = clipped_output.add_edge();
                        clipped_edge_type &new_twin_edge = clipped_output.add_edge();

                        // Update twin pointers.
                        new_edge.twin = &new_twin_edge;
                        new_twin_edge.twin = &new_edge;

                        // Update clipped edge pointers.
                        cur_edge->clipped_edge = &new_edge;
                        cur_twin_edge->clipped_edge = &new_twin_edge;

                        // Add new boundary vertex.
                        clipped_voronoi_record_type &new_vertex1 =
                            clipped_output.add_boundary_vertex(intersections[0]);
                        new_vertex1.incident_edge = &new_edge;
                        new_vertex1.num_incident_edges = 1;

                        // Add new boundary vertex
                        clipped_voronoi_record_type &new_vertex2 =
                            clipped_output.add_boundary_vertex(intersections[1]);
                        new_vertex2.incident_edge = &new_twin_edge;
                        new_vertex2.num_incident_edges = 1;

                        // Update edge pointers.
                        new_edge.start_point = new_twin_edge.end_point = &new_vertex1;
                        new_edge.end_point = new_twin_edge.start_point = &new_vertex2;
                        new_edge.rot_next = new_edge.rot_prev = &new_edge;
                        new_twin_edge.rot_next = new_twin_edge.rot_prev = &new_twin_edge;
                    }
                }
            } else {
            // Iterate over all voronoi vertices.
            for (voronoi_const_iterator_type vertex_it = vertex_records_.begin();
                 vertex_it != vertex_records_.end(); vertex_it++) {
                edge_type *cur_edge = vertex_it->incident_edge;
                const Point2D &cur_vertex_point = vertex_it->voronoi_point;

                // Check if bounding rectangle of clipped output contains current voronoi vertex.
                if (brect.contains(cur_vertex_point)) {
                    // Add current voronoi vertex to clipped output.
                    clipped_voronoi_record_type &new_vertex =
                        clipped_output.add_vertex(cur_vertex_point);
                    new_vertex.num_incident_edges = vertex_it->num_incident_edges;
                    clipped_edge_type *rot_prev_edge = NULL;

                    // Process all half-edges incident to the current voronoi vertex.
                    do {
                        // Add new edge to the clipped output.
                        clipped_edge_type &new_edge = clipped_output.add_edge();
                        new_edge.start_point = &new_vertex;
                        cur_edge->clipped_edge = &new_edge;
 
                        // Ray edges with no start point don't correspond to any voronoi vertex.
                        // That's why ray edges are processed during their twin edge processing.
                        if (cur_edge->end_point == NULL) {
                            // Add new twin edge.
                            clipped_edge_type &new_twin_edge = clipped_output.add_edge();
                            cur_edge->twin->clipped_edge = &new_twin_edge;
                            
                            // Add boundary vertex.
                            std::vector<Point2D> intersections;
                            const Point2D &site1 = cur_edge->cell->voronoi_point;
                            const Point2D &site2 = cur_edge->twin->cell->voronoi_point;
                            Point2D direction(site1.y() - site2.y(), site2.x() - site1.x());
                            find_intersections(cur_vertex_point, direction, RAY, brect, intersections);
                            clipped_voronoi_record_type &boundary_vertex = 
                                clipped_output.add_boundary_vertex(intersections[0]);
                            boundary_vertex.incident_edge = &new_twin_edge;
                            boundary_vertex.num_incident_edges = 1;

                            // Update new twin edge pointers.
                            new_twin_edge.start_point = &boundary_vertex;
                            new_twin_edge.rot_next = &new_twin_edge;
                            new_twin_edge.rot_prev = &new_twin_edge;
                        }

                        // Update twin and end point pointers.
                        clipped_edge_type *twin = cur_edge->twin->clipped_edge;
                        if (twin != NULL) {
                            new_edge.twin = twin;
                            twin->twin = &new_edge;
                            new_edge.end_point = twin->start_point;
                            twin->end_point = new_edge.start_point;
                        }

                        // Update rotation prev/next pointers.
                        if (rot_prev_edge != NULL) {
                            new_edge.rot_prev = rot_prev_edge;
                            rot_prev_edge->rot_next = &new_edge;
                        }

                        rot_prev_edge = &new_edge;
                        cur_edge = cur_edge->rot_next;
                    } while(cur_edge != vertex_it->incident_edge);
                    
                    // Update rotation prev/next pointers.
                    cur_edge->clipped_edge->rot_prev = rot_prev_edge;
                    rot_prev_edge->rot_next = cur_edge->clipped_edge;
                    new_vertex.incident_edge = cur_edge->clipped_edge;
                } else {
                    do {
                        std::vector<Point2D> intersections;
                        Point2D direction;
                        kEdgeType etype;
                        if (cur_edge->end_point != NULL) {
                            const Point2D &end_point = cur_edge->end_point->voronoi_point;
                            direction = make_point_2d<coordinate_type> (
                                end_point.x() - cur_vertex_point.x(),
                                end_point.y() - cur_vertex_point.y());
                            etype = SEGMENT;
                        } else {
                            const Point2D &site1 = cur_edge->cell->voronoi_point;
                            const Point2D &site2 = cur_edge->twin->cell->voronoi_point;
                            direction = make_point_2d<coordinate_type> (
                                site1.y() - site2.y(), site2.x() - site1.x());
                            etype = RAY;
                        }

                        // Find all intersections of the current
                        // edge with bounding box of the clipped output.
                        bool corner_intersection = find_intersections(cur_vertex_point, direction,
                                                                      etype, brect, intersections);

                        // Process edge if there are any intersections.
                        if (!corner_intersection && !intersections.empty()) {
                            // Add new vertex to the clipped output.
                            clipped_voronoi_record_type &new_vertex = 
                                clipped_output.add_boundary_vertex(intersections[0]);
                            new_vertex.num_incident_edges = 1;
    
                            // Add new edge to the clipped output.
                            clipped_edge_type &new_edge = clipped_output.add_edge();
                            new_edge.start_point = &new_vertex;
                            cur_edge->clipped_edge = &new_edge;

                            // Process twin ray edge with no start point.
                            if (cur_edge->end_point == NULL && intersections.size() == 2) {
                                clipped_edge_type &new_twin_edge = clipped_output.add_edge();
                                cur_edge->twin->clipped_edge = &new_twin_edge;

                                clipped_voronoi_record_type &boundary_vertex =
                                    clipped_output.add_boundary_vertex(intersections[1]);
                                boundary_vertex.incident_edge = &new_twin_edge;
                                boundary_vertex.num_incident_edges = 1;

                                new_twin_edge.start_point = &boundary_vertex;
                                new_twin_edge.rot_next = &new_twin_edge;
                                new_twin_edge.rot_prev = &new_twin_edge;
                            }   

                            // Update twin and end point pointers.
                            clipped_edge_type *twin = cur_edge->twin->clipped_edge;
                            if (twin != NULL) {
                                new_edge.twin = twin;
                                twin->twin = &new_edge;
                                new_edge.end_point = twin->start_point;
                                twin->end_point = new_edge.start_point;
                            }

                            // Update rotation prev/next pointers.
                            new_edge.rot_next = &new_edge;
                            new_edge.rot_prev = &new_edge;

                            new_vertex.incident_edge = cur_edge->clipped_edge;
                        }
                        cur_edge = cur_edge->rot_next;
                    } while (cur_edge != vertex_it->incident_edge);
                }
            }
            }

            // Iterate over all voronoi cells.
            for (voronoi_const_iterator_type cell_it = cell_records_.begin();
                 cell_it != cell_records_.end(); cell_it++) {
                // Add new cell to the clipped output.
                clipped_voronoi_record_type &new_cell =
                    clipped_output.add_cell(cell_it->voronoi_point);
                edge_type *cur_edge = cell_it->incident_edge;
                clipped_edge_type *prev = NULL;

                // Update cell, next/prev pointers.
                do {
                    clipped_edge_type *new_edge = cur_edge->clipped_edge;
                    if (new_edge) {
                        if (prev) {
                            new_edge->prev = prev;
                            prev->next = new_edge;
                        }
                        new_edge->cell = &new_cell;
                        if (new_cell.incident_edge == NULL)
                            new_cell.incident_edge = cur_edge->clipped_edge;
                        new_cell.num_incident_edges++;
                        prev = new_edge;
                        cur_edge->clipped_edge = NULL;
                    }
                    cur_edge = cur_edge->next;
                } while(cur_edge != cell_it->incident_edge);

                if (new_cell.incident_edge == NULL)
                    clipped_output.pop_cell();
                else {
                    // Update prev/next pointers.
                    prev->next = new_cell.incident_edge;
                    new_cell.incident_edge->prev = prev;
                }
            }
        }

        // Find edge(segment, ray, line) rectangle intersetion points.
        bool find_intersections(const Point2D &origin, const Point2D &direction,
            kEdgeType edge_type, const BRect<coordinate_type> &brect,
            std::vector<Point2D> &intersections) const {
            coordinate_type half = static_cast<coordinate_type>(0.5);

            // Find rectangle center.
            coordinate_type center_x = (brect.x_min + brect.x_max) * half;
            coordinate_type center_y = (brect.y_min + brect.y_max) * half;

            // Find rectangle half-diagonal vector.
            coordinate_type len_x = brect.x_max - center_x;
            coordinate_type len_y = brect.y_max - center_y;
            
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
            coordinate_type fT0 = static_cast<coordinate_type>(0);
            coordinate_type fT1 = static_cast<coordinate_type>(0);

            // Find intersections with lines going through sides of a bounding rectangle.
            clip(+direction.x(), -diff_x - len_x, fT0_used, fT1_used, fT0, fT1);
            clip(-direction.x(), +diff_x - len_x, fT0_used, fT1_used, fT0, fT1);
            clip(+direction.y(), -diff_y - len_y, fT0_used, fT1_used, fT0, fT1);
            clip(-direction.y(), +diff_y - len_y, fT0_used, fT1_used, fT0, fT1);

            if (fT0_used && check_extent(fT0, edge_type))
                intersections.push_back(make_point_2d(origin.x() + fT0 * direction.x(),
                                                      origin.y() + fT0 * direction.y()));
            if (fT1_used && fT0 != fT1 && check_extent(fT1, edge_type))
                intersections.push_back(make_point_2d(origin.x() + fT1 * direction.x(),
                                                      origin.y() + fT1 * direction.y()));

            return fT0_used && fT1_used && (fT0 == fT1);
        }

    private:
        // Simplify degenerate edge.
        void simplify_edge(edge_type *edge) {
            voronoi_record_type *vertex1 = edge->start_point;
            voronoi_record_type *vertex2 = edge->end_point;

            // Update number of incident edges.
            vertex1->num_incident_edges += vertex2->num_incident_edges - 2;

            // Update second vertex incident edges start and end points.
            edge_type *updated_edge = edge->twin->rot_next;
            while (updated_edge != edge->twin) {
                updated_edge->start_point = vertex1;
                updated_edge->twin->end_point = vertex1;
                updated_edge = updated_edge->rot_next;
            }

            edge_type *edge1 = edge;
            edge_type *edge2 = edge->twin;

            edge_type *edge1_rot_prev = edge1->rot_prev;
            edge_type *edge1_rot_next = edge1->rot_next;

            edge_type *edge2_rot_prev = edge2->rot_prev;
            edge_type *edge2_rot_next = edge2->rot_next;

            // Update prev and next pointers of incident edges.
            edge1_rot_next->twin->next = edge2_rot_prev;
            edge2_rot_prev->prev = edge1_rot_next->twin;
            edge1_rot_prev->prev = edge2_rot_next->twin;
            edge2_rot_next->twin->next = edge1_rot_prev;

            // Update rotation prev and next pointers of incident edges.
            edge1_rot_prev->rot_next = edge2_rot_next;
            edge2_rot_next->rot_prev = edge1_rot_prev;
            edge1_rot_next->rot_prev = edge2_rot_prev;
            edge2_rot_prev->rot_next = edge1_rot_next;

            // Change incident edge pointer of a vertex if it correspongs to the
            // degenerate edge.
            if (vertex1->incident_edge == edge)
                vertex1->incident_edge = edge->rot_prev;

            // Remove second vertex from the vertex records list.
            vertex_records_.erase(vertex2->voronoi_record_it);
        }

        bool check_extent(coordinate_type extent, kEdgeType etype) const {
            switch (etype) {
                case SEGMENT: return extent >= static_cast<coordinate_type>(0) &&
                                     extent <= static_cast<coordinate_type>(1);
                case RAY: return extent >= static_cast<coordinate_type>(0);
                case LINE: return true;
            }
            return true;
        }

        coordinate_type magnitude(coordinate_type value) const {
            if (value >= static_cast<coordinate_type>(0))
                return value;
            return -value;
        }

        bool clip(coordinate_type denom, coordinate_type numer, bool &fT0_used, bool &fT1_used,
            coordinate_type &fT0, coordinate_type &fT1) const {
            if (denom > static_cast<coordinate_type>(0)) {
                if (fT1_used && numer > denom * fT1)
                    return false;
                if (!fT0_used || numer > denom * fT0) {
                    fT0_used = true;
                    fT0 = numer / denom;
                }
                return true;
            } else if (denom < static_cast<coordinate_type>(0)) {
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

        std::vector<voronoi_iterator_type> cell_iterators_;
        voronoi_records_type cell_records_;
        voronoi_records_type vertex_records_;
        std::list<edge_type> edges_;

        int num_cell_records_;
        int num_vertex_records_;
        int num_edges_;

        BRect<coordinate_type> voronoi_rect_;

        // Disallow copy constructor and operator=
        voronoi_output(const voronoi_output&);
        void operator=(const voronoi_output&);
    };
  
} // sweepline
} // boost
} // detail

#endif

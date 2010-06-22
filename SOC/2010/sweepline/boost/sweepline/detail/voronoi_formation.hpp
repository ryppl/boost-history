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
    struct site_event;
    
    template <typename T>
    struct circle_event;

    template <typename Point2D>
    struct beach_line_node;

    template <typename Point2D>
    struct beach_line_node_data;

    template <typename BeachLineNode>
    struct node_comparer;

    // Point in 2D space data structure. Comparators defined in this
    // data structure actually define sweepline movement direction.
    template <typename T>
    struct point_2d {
    public:
        typedef T coordinate_type;
        typedef site_event<T> site_event_type;
        typedef circle_event<T> circle_event_type;

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

    // Site event type. 
    // Occurs when sweepline sweeps over one of the initial sites.
    // Contains index of a site below the other sorted sites.
    template <typename T>
    struct site_event {
    public:
        typedef T coordinate_type;

        site_event() {}
        
        site_event(T x, T y, int index) : point_(x, y), site_index_(index) {}

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

        const point_2d<T> &get_point() const {
            return point_;
        }

        int get_site_index() const {
            return site_index_;
        }

    private:
        point_2d<T> point_;
        int site_index_;
    };

    template <typename T>
    site_event<T> make_site_event(T x, T y, int index) {
        return site_event<T>(x, y, index);
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
        typedef typename beach_line_node< point_2d<T> > Key;
        typedef typename beach_line_node_data< point_2d<T> > Value;
        typedef typename node_comparer<Key> NodeComparer;
        typedef typename std::map< Key, Value, NodeComparer >::const_iterator beach_line_iterator;

        circle_event() {}

        circle_event(T c_x, T c_y, T sqr_r) :
        center_(c_x, c_y), sqr_radius_(sqr_r) {}

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

            T sqr_dif_x = (center_.x() - c_event.x()) *
                          (center_.x() - c_event.x());
            T sum_r_sqr = sqr_radius_ + c_event.sqr_radius_;
            T value_left = (sum_r_sqr - sqr_dif_x) * (sum_r_sqr - sqr_dif_x);
            T value_right = static_cast<T>(4) * sqr_radius_ *
                            c_event.sqr_radius_;

            return value_left == value_right;
        }

        bool operator!=(const circle_event &c_event) const {
            return !((*this) == c_event);
        }

        bool operator<(const circle_event &c_event) const {
            T x1 = center_.x();
            T y1 = center_.y();
            T sqr_r1 = sqr_radius_;
            T x2 = c_event.x();
            T y2 = c_event.y();
            T sqr_r2 = c_event.get_sqr_radius();

            T sqr_dif_x = (x1 - x2) * (x1 - x2);
            T sum_r_sqr = sqr_r1 + sqr_r2;
            T value_left = (sum_r_sqr - sqr_dif_x) * (sum_r_sqr - sqr_dif_x);
            T value_right = static_cast<T>(4) * sqr_r1 * sqr_r2;
            
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
        int compare(const site_event<T> &s_event) const {
            if (s_event.x() < center_.x())
                return 1;
            T sqr_dif_x = (s_event.x() - center_.x()) * (s_event.x() - center_.x());
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

        const point_2d<T> &get_center() const {
            return center_;
        }

        const T &get_sqr_radius() const {
            return sqr_radius_;
        }

        void set_bisector(beach_line_iterator iterator) {
            bisector_node_ = iterator;
        }

        void set_sites(const site_event<T> site1,
                       const site_event<T> site2,
                       const site_event<T> site3) {
            sites_[0] = site1;
            sites_[1] = site2;
            sites_[2] = site3;
        }

        const beach_line_iterator &get_bisector() const {
            return bisector_node_;
        }

        const site_event<T>* get_sites() const {
            return sites_;
        }

    private:
        point_2d<T> center_;
        T sqr_radius_;
        beach_line_iterator bisector_node_;
        site_event<T> sites_[3];
    };

    template <typename T>
    circle_event<T> make_circle_event(T center_x, T center_y, T sqr_radius) {
        return circle_event<T>(center_x, center_y, sqr_radius);
    }

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT TYPES ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    template <typename Point2D>
    struct half_edge;

    // Cell record data structure. Represents voronoi cell.
    // Contains site point and pointer to any incident half-edge.
    template <typename Point2D>
    struct cell_record {
        half_edge<Point2D> *incident_edge;
        Point2D site_point;

        cell_record(Point2D site, half_edge<Point2D>* edge) : incident_edge(edge), site_point(site) {}
    };

    // Voronoi vertex data structure. Represents voronoi vertex.
    // Contains vertex coordinates and pointers to all incident half-edges.
    template <typename Point2D>
    struct vertex_record {
        std::list< half_edge<Point2D>* > incident_edges;
        Point2D vertex;

        vertex_record(Point2D vertex) : vertex(vertex) {}
    };

    // Half-edge data structure. Represents voronoi edge.
    // Contains: 1) pointer to cell records;
    //           2) pointers to start/end vertices of half-edge;
    //           3) pointers to previous/next half-edges(CCW);
    //           4) pointer to twin half-edge.
    template <typename Point2D>
    struct half_edge {
        typedef typename cell_record<Point2D> cell_record_type;
        typedef typename vertex_record<Point2D> vertex_record_type;
        typedef typename half_edge<Point2D> half_edge_type;

        cell_record_type *cell_record;
        vertex_record_type *start_point;
        vertex_record_type *end_point;
        half_edge_type *prev;
        half_edge_type *next;
        half_edge_type *twin;

        half_edge(int site_index) :
            cell_record(NULL),
            start_point(NULL),
            end_point(NULL),
            prev(NULL),
            next(NULL),
            twin(NULL) {}
    };

    // Voronoi output data structure based on the half-edges.
    // Contains vector of voronoi cells, doubly linked list of
    // voronoi vertices and voronoi edges.
    template <typename Point2D>
    class voronoi_output {
    public:
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;
        typedef typename cell_record<Point2D> cell_record_type;
        typedef typename vertex_record<Point2D> vertex_record_type;
        typedef typename half_edge<Point2D> edge_type;
        typedef typename std::vector<cell_record_type> cell_records;
        typedef typename std::list<vertex_record_type> voronoi_vertices;
        typedef typename std::list<edge_type *>::const_iterator edge_iterator;
        typedef typename voronoi_vertices::const_iterator voronoi_vertices_iterator;

        voronoi_output() {}

        // This preserves the validity of iterators.
        void init(int num_sites) {
            cell_records_.reserve(num_sites);
        }

        void reset() {
            cell_records_.clear();
            vertex_records_.clear();
            edges_.clear();
        }

        // Inserts new half-edge into the output data structure during site
        // event processing. Takes as input left and right sites of the new
        // beach line node and returns reference to the new half-edge. 
        // Second argument is new site. During this step we add two new
        // twin half-edges.
        edge_type *insert_new_edge(const site_event_type &site1,
                                   const site_event_type &site2) {
            // Get indexes of sites.                                           
            int site_index1 = site1.get_site_index();
            int site_index2 = site2.get_site_index();

            // Create new half-edge that belongs to the cell with the first site.
            edges_.push_back(edge_type(site_index1));
            edge_type &edge1 = edges_.back();

            // Create new half-edge that belongs to the cell with the second site.
            edges_.push_back(edge_type(site_index2));
            edge_type &edge2 = edges_.back();

            // Add initial cell during first edge insertion.
            if (cell_records_.empty())
                cell_records_.push_back(cell_record_type(site1.get_point(), &edge1));

            // Second site represents new site during site event processing.
            // Add new cell to the cell records vector.
            cell_records_.push_back(cell_record_type(site2.get_point(), &edge2));

            // Update pointers to cells.
            edge1.cell_record = &cell_records_[site_index1];
            edge2.cell_record = &cell_records_[site_index2];

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
            // Add new voronoi vertex as voronoi circle center.
            vertex_records_.push_back(vertex_record_type(circle.get_center()));
            vertex_record_type &new_vertex = vertex_records_.back();

            // Update two input bisectors and their twins half-edges with
            // new voronoi vertex.
            edge12->start_point = &new_vertex;
            edge12->twin->end_point = &new_vertex;
            edge23->start_point = &new_vertex;
            edge23->twin->end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type(site1.get_site_index()));
            edge_type &new_edge1 = edges_.back();
            new_edge1.cell_record = &cell_records_[site1.get_site_index()];
            new_edge1.end_point = &new_vertex;

            // Add new half-edge.
            edges_.push_back(edge_type(site3.get_site_index()));
            edge_type &new_edge2 = edges_.back();
            new_edge2.cell_record = &cell_records_[site3.get_site_index()];
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

            // Update voronoi vertex incident edges pointers.
            new_vertex.incident_edges.push_back(edge12);
            new_vertex.incident_edges.push_back(edge23);
            new_vertex.incident_edges.push_back(&new_edge2);
            return &new_edge1;
        }

        const cell_records &get_cell_records() const {
            return cell_records_;
        }

        const voronoi_vertices &get_voronoi_vertices() const {
            return vertex_records_;
        }

    private:
        std::vector<cell_record_type> cell_records_;
        std::list<vertex_record_type> vertex_records_;
        std::list<edge_type> edges_;

        //Disallow copy constructor and operator=
        voronoi_output(const voronoi_output&);
        void operator=(const voronoi_output&);
    };

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI CIRCLE EVENTS QUEUE ////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    // Event queue data structure, processes circle events.
    template <typename Point2D>
    class circle_events_queue {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::circle_event_type circle_event_type;

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

    /////////////////////////////////////////////////////////////////////////////
    // VORONOI BEACH LINE TYPES /////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    
    // Represents bisector made by two arcs that correspond to the left and
    // right sites. Arc is defined as curve with points equidistant from the
    // site and from the sweepline.
    // Let sweepline sweep from left to right and it's current coordinate
    // be x0, site coordinates be (x1, y1). In this case equation of the arc
    // may be written as: (x-x0)^2 = (x-x1)^2 + (y-y1)^2, or
    // x = ((y - y1)^2 + x1^2 - x0^2) / (2*(x1 - x0)).
    // In general case two arcs will create two different bisectors. That's why
    // the order of arcs is important to define unique bisector.
    template <typename Point2D>
    struct beach_line_node {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;

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
            return std::max(left_site_.x(), right_site_.x());
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

    // Represents edge data sturcture (bisector segment), which is
    // associated with beach line node key in the binary search tree.
    template <typename Point2D>
    struct beach_line_node_data {
        typedef typename half_edge<Point2D> Edge;

        Edge *edge;

        beach_line_node_data(Edge *new_edge) : edge(new_edge) {}

        void change_edge(Edge *new_edge) {
            edge = new_edge;
        }
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
  
} // sweepline
} // boost
} // detail

#endif

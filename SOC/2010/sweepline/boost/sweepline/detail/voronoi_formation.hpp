// Boost sweepline/voronoi_formation.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_FORMATION
#define BOOST_SWEEPLINE_VORONOI_FORMATION

#define INT_PREDICATE_COMPUTE_DIFFERENCE(a, b, res, sign) \
        if (a >= b) { res = static_cast<ull>(a - b); sign = true; } \
        else { res = static_cast<ull>(b - a); sign = false; }

#define INT_PREDICATE_AVOID_CANCELLATION(val, first_expr, second_expr) \
        if ((val) >= 0) first_expr += (val); \
        else second_expr -= (val);

namespace boost {
namespace sweepline {
namespace detail {

    ///////////////////////////////////////////////////////////////////////////
    // GEOMETRY PREDICATES ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // If two floating-point numbers in the same format are ordered (x < y), then
    // they are ordered the same way when their bits are reinterpreted as
    // sign-magnitude integers.
    bool almost_equal(double a, double b, long long maxUlps) {
        long long ll_a, ll_b;
        // Reinterpret double bits as long long.
        memcpy(&ll_a, &a, sizeof(double));
        memcpy(&ll_b, &b, sizeof(double));

        // Positive 0.0 is integer zero. Negative 0.0 is 0x8000000000000000.
        // Map negative zero to an integer zero representation - making it
        // identical to positive zero - and it makes it so that the smallest
        // negative number is represented by negative one, and downwards from there.
        if (ll_a < 0)
            ll_a = 0x8000000000000000LL - ll_a;
        if (ll_b < 0)
            ll_b = 0x8000000000000000LL - ll_b;

        // Compare long long representations of input values.
        // Difference in 1 Ulp is equivalent to a relative error of between
        // 1/4,000,000,000,000,000 and 1/8,000,000,000,000,000.
        long long dif = ll_a - ll_b;
        return (dif <= maxUlps) && (dif >= -maxUlps);
    }

    // TODO(asydorchuk): Make templates specification for integer coordinate type,
    // as it is actually working with integer input.
    template <typename T>
    bool right_orientation_test(const point_2d<T> &point1,
                                const point_2d<T> &point2,
                                const point_2d<T> &point3) {
        typedef long long ll;
        typedef unsigned long long ull;
        ull dif_x1, dif_x2, dif_y1, dif_y2;
        bool dif_x1_plus, dif_x2_plus, dif_y1_plus, dif_y2_plus;
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(point1.x()),
                                         static_cast<ll>(point2.x()),
                                         dif_x1, dif_x1_plus);
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(point2.x()),
                                         static_cast<ll>(point3.x()),
                                         dif_x2, dif_x2_plus);
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(point1.y()),
                                         static_cast<ll>(point2.y()),
                                         dif_y1, dif_y1_plus);
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(point2.y()),
                                         static_cast<ll>(point3.y()),
                                         dif_y2, dif_y2_plus);
        ull expr_l = dif_x1 * dif_y2;
        bool expr_l_plus = (dif_x1_plus == dif_y2_plus) ? true : false;
        ull expr_r = dif_x2 * dif_y1;
        bool expr_r_plus = (dif_x2_plus == dif_y1_plus) ? true : false;

        if (expr_l == 0)
            expr_l_plus = true;
        if (expr_r == 0)
            expr_r_plus = true;
        
        if (!expr_l_plus) {
            if (expr_r_plus)
                return true;
            else
                return expr_l > expr_r; 
        } else {
            if (!expr_r_plus)
                return false;
            else
                return expr_l < expr_r;
        }
    }

    enum kPredicateResult {
        LESS = -1,
        UNDEFINED = 0,
        MORE = 1,
    };

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
    // (x0-x2)*(x0-x1)*(x1-x2) + (x0-x2)*(y*-y1)^2 < (x0-x1)*(y*-y2)^2.
    template <typename T>
    kPredicateResult fast_less_predicate(const point_2d<T> &left_point,
                                         const point_2d<T> &right_point,
                                         const point_2d<T> &new_point) {
        double fast_a1 = static_cast<double>(new_point.x()) - static_cast<double>(left_point.x());
        double fast_a2 = static_cast<double>(new_point.x()) - static_cast<double>(right_point.x());
        double fast_b1 = static_cast<double>(new_point.y()) - static_cast<double>(left_point.y());
        double fast_b2 = static_cast<double>(new_point.y()) - static_cast<double>(right_point.y());
        double fast_c = static_cast<double>(left_point.x()) - static_cast<double>(right_point.x());
        double fast_left_expr = fast_a1 * fast_b2 * fast_b2;
        double fast_right_expr = fast_a2 * fast_b1 * fast_b1;
        
        // Avoid cancellation.
        INT_PREDICATE_AVOID_CANCELLATION(fast_a1 * fast_a2 * fast_c,
                                         fast_left_expr, fast_right_expr);
        if (!almost_equal(fast_left_expr, fast_right_expr, 5))
            return (fast_left_expr < fast_right_expr) ? LESS : MORE;
        return UNDEFINED;
    }

    template <typename T>
    bool less_predicate(const point_2d<T> &left_point,
                        const point_2d<T> &right_point,
                        const point_2d<T> &new_point) {
        kPredicateResult fast_res = fast_less_predicate(left_point, right_point, new_point);
        if (fast_res != UNDEFINED)
            return (fast_res == LESS);

        typedef long long ll;
        typedef unsigned long long ull;
        ull a1, a2, b1, b2, b1_sqr, b2_sqr, l_expr, r_expr;
        bool l_expr_plus, r_expr_plus;

        // a1 and a2 are greater than zero.
        a1 = static_cast<ull>(static_cast<ll>(new_point.x()) -
                              static_cast<ll>(left_point.x()));
        a2 = static_cast<ull>(static_cast<ll>(new_point.x()) -
                              static_cast<ll>(right_point.x()));

        // We don't need to know signs of b1 and b2, because we use their squared values.
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(new_point.y()),
                                         static_cast<ll>(left_point.y()),
                                         b1, l_expr_plus);
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(new_point.y()),
                                         static_cast<ll>(right_point.y()),
                                         b2, l_expr_plus);
        b1_sqr = b1 * b1;
        b2_sqr = b2 * b2;
        ull b1_sqr_mod = b1_sqr % a1;
        ull b2_sqr_mod = b2_sqr % a2;

        // Compute left expression.
        INT_PREDICATE_COMPUTE_DIFFERENCE(static_cast<ll>(left_point.x()),
                                         static_cast<ll>(right_point.x()),
                                         l_expr, l_expr_plus);            
        if (b2_sqr_mod * a1 < b1_sqr_mod * a2) {
            if (!l_expr_plus)
                l_expr++;
            else if (l_expr != 0)
                l_expr--;
            else {
                l_expr++;
                l_expr_plus = false;
            }
        }

        // Compute right expression.
        INT_PREDICATE_COMPUTE_DIFFERENCE(b1_sqr / a1, b2_sqr / a2, r_expr, r_expr_plus);

        // Compare left and right expressions.
        if (!l_expr_plus && r_expr_plus)
            return true;
        if (l_expr_plus && !r_expr_plus)
            return false;
        if (l_expr_plus && r_expr_plus)
            return l_expr < r_expr;
        return l_expr > r_expr;
    }

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI EVENT TYPES ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct beach_line_node;

    template <typename T>
    struct beach_line_node_data;

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
    // Circle event contains circle center, lowest x coordinate, event state and
    // iterator to the corresponding bisectors.
    template <typename T>
    struct circle_event {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
        typedef beach_line_node<coordinate_type> Key;
        typedef beach_line_node_data<coordinate_type> Value;
        typedef node_comparer<Key> NodeComparer;
        typedef typename std::map< Key, Value, NodeComparer >::iterator beach_line_iterator;

        circle_event() : is_active_(true) {}

        circle_event(coordinate_type c_x, coordinate_type c_y, coordinate_type lower_x) :
            center_(c_x, c_y), lower_x_(lower_x), is_active_(true) {}

        circle_event(const Point2D &center, coordinate_type lower_x) :
            center_(center), lower_x_(lower_x), is_active_(true) {}

        circle_event(const circle_event& c_event) {
            center_ = c_event.center_;
            lower_x_ = c_event.lower_x_;
            bisector_node_ = c_event.bisector_node_;
            is_active_ = c_event.is_active_;
        }

        void operator=(const circle_event& c_event) {
            center_ = c_event.center_;
            lower_x_ = c_event.lower_x_;
            bisector_node_ = c_event.bisector_node_;
            is_active_ = c_event.is_active_;
        }

        bool operator==(const circle_event &c_event) const {
            return (center_.y() == c_event.y()) &&
                   (center_.x() == c_event.x()) &&
                   (lower_x_ == c_event.lower_x_);
        }

        bool operator!=(const circle_event &c_event) const {
            return !((*this) == c_event);
        }

        bool operator<(const circle_event &c_event) const {
            if (lower_x_ != c_event.lower_x_)
                return lower_x_ < c_event.lower_x_;
            return center_.y() < c_event.y();
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
        // If circle point is less than site point return -1.
        // If circle point is equal to site point return 0.
        // If circle point is greater than site point return 1.
        int compare(const site_event<coordinate_type> &s_event) const {
            if (s_event.x() != lower_x_) {
                return (lower_x_ < s_event.x()) ? -1 : 1;
            }
            if (s_event.y() != center_.y())
                return (center_.y() < s_event.y()) ? -1 : 1;
            return 0;
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

        const coordinate_type &get_lower_x() const {
            return lower_x_;
        }

        void set_bisector(beach_line_iterator iterator) {
            bisector_node_ = iterator;
        }

        void deactivate() {
            is_active_ = false;
        }

        const beach_line_iterator &get_bisector() const {
            return bisector_node_;
        }

        bool is_active() const {
            return is_active_;
        }

    private:
        Point2D center_;
        coordinate_type lower_x_;
        beach_line_iterator bisector_node_;
        bool is_active_;
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
        typedef typename std::list<circle_event_type>::iterator circle_event_type_it;

        circle_events_queue() {}

        void reset() {
            while (!circle_events_.empty())
                circle_events_.pop();
            circle_events_list_.clear();
        }

        bool empty() {
            remove_not_active_events();
            return circle_events_.empty();
        }

        const circle_event_type &top() {
            remove_not_active_events();
            return (*circle_events_.top());
        }

        void pop() {
            circle_event_type_it temp_it = circle_events_.top();
            circle_events_.pop();
            circle_events_list_.erase(temp_it);
        }

        circle_event_type_it push(const circle_event_type &c_event) {
            circle_events_list_.push_front(c_event);
            circle_events_.push(circle_events_list_.begin());
            return circle_events_list_.begin();
        }

    private:
        struct comparison {
            bool operator() (const circle_event_type_it &node1,
                             const circle_event_type_it &node2) const {
                return (*node1) > (*node2);
            }
        };

        void remove_not_active_events() {
            while (!circle_events_.empty() && !circle_events_.top()->is_active())
                pop();
        }

        std::priority_queue< circle_event_type_it,
                             std::vector<circle_event_type_it>,
                             comparison > circle_events_;
        std::list<circle_event_type> circle_events_list_;

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

        // Returns the rightmost site.
        const site_event_type& get_new_site() const {
            if (left_site_.x() > right_site_.x())
                return left_site_;
            return right_site_;
        }

        bool less(const Point2D &new_site) const {
            if (left_site_.x() > right_site_.x()) {
                if (new_site.y() <= left_site_.y())
                    return false;
                return less_predicate(left_site_.get_point(), right_site_.get_point(), new_site);
            } else if (left_site_.x() < right_site_.x()) {
                if (new_site.y() >= right_site_.y())
                    return true;
                return less_predicate(left_site_.get_point(), right_site_.get_point(), new_site);
            } else {
                return left_site_.y() + right_site_.y() <
                       static_cast<coordinate_type>(2.0) * new_site.y();
            }
        }

    private:
        site_event_type left_site_;
        site_event_type right_site_;
    };

    template <typename T>
    struct half_edge;

    // Represents edge data sturcture (bisector segment), which is
    // associated with beach line node key in the binary search tree.
    template <typename T>
    struct beach_line_node_data {
        half_edge<T> *edge;
        typename circle_events_queue<T>::circle_event_type_it circle_event_it;
        bool contains_circle_event;

        explicit beach_line_node_data(half_edge<T> *new_edge) :
            edge(new_edge),
            contains_circle_event(false) {}

        void activate_circle_event(typename circle_events_queue<T>::circle_event_type_it it) {
            circle_event_it = it;
            contains_circle_event = true;
        }

        void deactivate_circle_event() {
            if (contains_circle_event)
                circle_event_it->deactivate();
            contains_circle_event = false;
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
            coordinate_type node1_line = node1.get_new_site().x();
            coordinate_type node2_line = node2.get_new_site().x();

            if (node1_line < node2_line) {
                return node1.less(node2.get_new_site().get_point());
            } else if (node1_line > node2_line) {
                return !node2.less(node1.get_new_site().get_point());
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
            if (!brect.is_valid())
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

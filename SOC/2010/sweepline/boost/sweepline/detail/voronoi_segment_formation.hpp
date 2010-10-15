// Boost sweepline/voronoi_segment_formation.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SEGMENT_FORMATION
#define BOOST_SWEEPLINE_VORONOI_SEGMENT_FORMATION

#define INT_PREDICATE_COMPUTE_DIFFERENCE(a, b, res, sign) \
        if (a >= b) { res = static_cast<unsigned long long>(a - b); sign = true; } \
        else { res = static_cast<unsigned long long>(b - a); sign = false; }

#define INT_PREDICATE_CONVERT_65_BIT(a, res, sign) \
        if (a >= 0) { res = static_cast<unsigned long long>(a); sign = true; } \
        else { res = static_cast<unsigned long long>(-a); sign = false; }

#define INT_PREDICATE_AVOID_CANCELLATION(val, first_expr, second_expr) \
        if ((val) >= 0) first_expr += (val); \
        else second_expr -= (val);

namespace boost {
namespace sweepline {
namespace detail {

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI EVENT TYPES ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct beach_line_node;

    template <typename T>
    struct beach_line_node_data;

    template <typename BeachLineNode>
    struct node_comparer;

    enum kOrientation {
        RIGHT_ORIENTATION = -1,
        COLINEAR = 0,
        LEFT_ORIENTATION = 1,
    };

    // Site event type. 
    // Occurs when sweepline sweeps over one of the initial sites.
    // Contains index of a site among the other sorted sites.
    template <typename T>
    struct site_event {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;

        site_event() : is_segment_(false),
                       is_vertical_(true),
                       is_inverse_(false) {}
        
        site_event(coordinate_type x, coordinate_type y, int index) :
            point0_(x, y),
            site_index_(index), 
            is_segment_(false),
            is_vertical_(true),
            is_inverse_(false) {}

        site_event(const Point2D &point, int index) :
            point0_(point),
            site_index_(index),
            is_segment_(false),
            is_vertical_(true),
            is_inverse_(false) {}

        site_event(const Point2D &point1, const Point2D &point2, int index) :
            point0_(point1),
            point1_(point2),
            site_index_(index),
            is_segment_(true),
            is_inverse_(false) {
            if (point0_ > point1_)
                (std::swap)(point0_, point1_);
            is_vertical_ = (point0_.x() == point1_.x());
        }

        bool operator==(const site_event &s_event) const {
            return (point0_ == s_event.point0_) &&
                   ((!is_segment_ && !s_event.is_segment_) ||
                   (is_segment_ && s_event.is_segment_ && (point1_ == s_event.point1_)));
        }

        bool operator!=(const site_event &s_event) const {
            return !((*this) == s_event);
        }

        // All the sites are sorted due to coordinates of the first point.
        // Point sites preceed vertical segment sites with the same first point.
        // Point sites and vertical segments preceed not vertical segments with
        // the same x coordinate of the first point.
        // Non vertical segments with the same first point are sorted counterclockwise.
        bool operator<(const site_event &s_event) const {
            // If first points have different x coordinates, compare them.
            if (this->point0_.x() != s_event.point0_.x())
                return this->point0_.x() < s_event.point0_.x();

            if (!(this->is_segment_)) {
                if (!s_event.is_segment_) {
                    return this->point0_.y() < s_event.point0_.y();
                }
                if (s_event.is_vertical_) {
                    return this->point0_.y() <= s_event.point0_.y();
                }
                return true;
            } else {
                if (!s_event.is_segment_ || s_event.is_vertical_) {
                    if (this->is_vertical_) {
                        return this->point0_.y() < s_event.point0_.y();
                    }
                    return false;
                }
                if (this->is_vertical_)
                    return true;
                if (this->point0_.y() != s_event.point0_.y())
                    return this->point0_.y() < s_event.point0_.y();
                // Sort by angle.
                return orientation_test(this->point1_, this->point0_, s_event.point1_) ==
                       LEFT_ORIENTATION;
            }   
        }

        bool operator<=(const site_event &s_event) const {
            return !(s_event < (*this));
        }

        bool operator>(const site_event &s_event) const {
            return s_event < (*this);
        }

        bool operator>=(const site_event &s_event) const {
            return !((*this) < s_event);
        }

        coordinate_type x(bool oriented = false) const {
            if (!oriented)
                return point0_.x();
            return is_inverse_ ? point1_.x() : point0_.x();
        }

        coordinate_type y(bool oriented = false) const {
            if (!oriented)
                return point0_.y();
            return is_inverse_ ? point1_.y() : point0_.y();
        }

        coordinate_type x0(bool oriented = false) const {
            if (!oriented)
                return point0_.x();
            return is_inverse_ ? point1_.x() : point0_.x();
        }

        coordinate_type y0(bool oriented = false) const {
            if (!oriented)
                return point0_.y();
            return is_inverse_ ? point1_.y() : point0_.y();
        }

        coordinate_type x1(bool oriented = false) const {
            if (!oriented)
                return point1_.x();
            return is_inverse_ ? point0_.x() : point1_.x();
        }

        coordinate_type y1(bool oriented = false) const {
            if (!oriented)
                return point1_.y();
            return is_inverse_ ? point0_.y() : point1_.y();
        }

        const Point2D &get_point0(bool oriented = false) const {
            if (!oriented)
                return point0_;
            return is_inverse_ ? point1_ : point0_;
        }

        const Point2D &get_point1(bool oriented = false) const {
            if (!oriented)
                return point1_;
            return is_inverse_ ? point0_ : point1_;
        }

        void set_site_index(int index) {
            site_index_ = index;
        }

        void set_inverse() {
            is_inverse_ ^= true;
        }

        int get_site_index() const {
            return site_index_;
        }

        bool is_segment() const {
            return is_segment_;
        }

        bool is_vertical() const {
            return is_vertical_;
        }

        bool is_inverse() const {
            return is_inverse_;
        }

    private:
        Point2D point0_;
        Point2D point1_;
        int site_index_;
        bool is_segment_;
        bool is_vertical_;
        bool is_inverse_;
    };

    template <typename T>
    site_event<T> make_site_event(T x, T y, int index) {
        return site_event<T>(x, y, index);
    }

    template <typename T>
    site_event<T> make_site_event(const point_2d<T> &point, int index) {
        return site_event<T>(point, index);
    }

    template <typename T>
    site_event<T> make_site_event(const point_2d<T> &point1,
                                  const point_2d<T> &point2, int index) {
        return site_event<T>(point1, point2, index);
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
    circle_event<T> make_circle_event(T c_x, T c_y, T lower_x) {
        return circle_event<T>(c_x, c_y, lower_x);
    }

    template <typename T>
    circle_event<T> make_circle_event(point_2d<T> center, T lower_x) {
        return circle_event<T>(center, lower_x);
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
    kOrientation orientation_test(const point_2d<T> &point1,
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
        
        if ((expr_l_plus == expr_r_plus) && (expr_l == expr_r))
            return COLINEAR;

        if (!expr_l_plus) {
            if (expr_r_plus)
                return RIGHT_ORIENTATION;
            else
                return (expr_l > expr_r) ? RIGHT_ORIENTATION : LEFT_ORIENTATION; 
        } else {
            if (!expr_r_plus)
                return LEFT_ORIENTATION;
            else
                return (expr_l < expr_r) ? RIGHT_ORIENTATION : LEFT_ORIENTATION;
        }
    }

    kOrientation orientation_test(long long dif_x1_, long long dif_y1_,
                                  long long dif_x2_, long long dif_y2_) {
        typedef unsigned long long ull;
        ull dif_x1, dif_y1, dif_x2, dif_y2;
        bool dif_x1_plus, dif_x2_plus, dif_y1_plus, dif_y2_plus;
        INT_PREDICATE_CONVERT_65_BIT(dif_x1_, dif_x1, dif_x1_plus);
        INT_PREDICATE_CONVERT_65_BIT(dif_y1_, dif_y1, dif_y1_plus);
        INT_PREDICATE_CONVERT_65_BIT(dif_x2_, dif_x2, dif_x2_plus);
        INT_PREDICATE_CONVERT_65_BIT(dif_y2_, dif_y2, dif_y2_plus);
        
        ull expr_l = dif_x1 * dif_y2;
        bool expr_l_plus = (dif_x1_plus == dif_y2_plus) ? true : false;
        ull expr_r = dif_x2 * dif_y1;
        bool expr_r_plus = (dif_x2_plus == dif_y1_plus) ? true : false;

        if (expr_l == 0)
            expr_l_plus = true;
        if (expr_r == 0)
            expr_r_plus = true;
        
        if ((expr_l_plus == expr_r_plus) && (expr_l == expr_r))
            return COLINEAR;

        if (!expr_l_plus) {
            if (expr_r_plus)
                return RIGHT_ORIENTATION;
            else
                return (expr_l > expr_r) ? RIGHT_ORIENTATION : LEFT_ORIENTATION; 
        } else {
            if (!expr_r_plus)
                return LEFT_ORIENTATION;
            else
                return (expr_l < expr_r) ? RIGHT_ORIENTATION : LEFT_ORIENTATION;
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

    template <typename T>
    kPredicateResult fast_less_predicate(point_2d<T> site_point, site_event<T> segment_site,
                                         point_2d<T> new_point, bool reverse_order) {
        typedef long long ll;
        typedef unsigned long long ull;
        if (orientation_test(segment_site.get_point0(true), segment_site.get_point1(true),
            new_point) != RIGHT_ORIENTATION) {
            return (!segment_site.is_inverse()) ? LESS : MORE;
        }

        const point_2d<T> &segment_start = segment_site.get_point0();
        const point_2d<T> &segment_end = segment_site.get_point1();
        ll dif_x = static_cast<ll>(new_point.x()) - static_cast<ll>(site_point.x());
        ll dif_y = static_cast<ll>(new_point.y()) - static_cast<ll>(site_point.y());
        ll a = static_cast<ll>(segment_end.x()) - static_cast<ll>(segment_start.x());
        ll b = static_cast<ll>(segment_end.y()) - static_cast<ll>(segment_start.y());

        if (a == 0) {
            if (new_point.y() < site_point.y() && !reverse_order)
                return MORE;
            else if (new_point.y() > site_point.y() && reverse_order)
                return LESS;
            return UNDEFINED;
        } else {
            kOrientation orientation = orientation_test(a, b, dif_x, dif_y);
            if ((orientation == COLINEAR) ||
                (!segment_site.is_inverse() ^ (orientation == RIGHT_ORIENTATION))) {
                if (!segment_site.is_inverse())
                    return reverse_order ? LESS : UNDEFINED;
                return reverse_order ? UNDEFINED : MORE;
            }            
        }

        // dif_x and dif_y are integers, so there will be no cancellation issues.
        double fast_left_expr = static_cast<double>(a) *
                                static_cast<double>(dif_y + dif_x) *
                                static_cast<double>(dif_y - dif_x);
        double fast_right_expr = static_cast<double>(b<<1) *
                                 static_cast<double>(dif_x) *
                                 static_cast<double>(dif_y);
        if (!(almost_equal(fast_left_expr, fast_right_expr, 4))) {
            if (segment_site.is_inverse() ^ (fast_left_expr > fast_right_expr) ^ reverse_order)
                return reverse_order ? LESS : MORE;
            return UNDEFINED;
        }

        ull a_rob, a_sign, b_rob, b_sign, dif_x_rob, dif_x_sign, dif_y_rob, dif_y_sign;
        INT_PREDICATE_CONVERT_65_BIT(a, a_rob, a_sign);
        INT_PREDICATE_CONVERT_65_BIT(b, b_rob, b_sign);
        INT_PREDICATE_CONVERT_65_BIT(dif_x, dif_x_rob, dif_x_sign);
        INT_PREDICATE_CONVERT_65_BIT(dif_y, dif_y_rob, dif_y_sign);
        
        ull dif_x_sqr = dif_x_rob * dif_x_rob;
        ull dif_y_sqr = dif_y_rob * dif_y_rob;
        ull left_expr_div = dif_y_sqr / dif_x_sqr + 1;
        ull left_expr_mod = dif_y_sqr % dif_x_sqr;

        ull right_expr_denom = a_rob * dif_x_rob;
        ull right_expr = b_rob * dif_y_rob;
        ull right_expr_div = right_expr / right_expr_denom;
        ull right_expr_mod = right_expr % right_expr_denom;

        bool comparison_result;
        if ((b_sign != dif_y_sign) && right_expr_div) 
            comparison_result = true;
        else {
            if (b_sign != dif_y_sign && right_expr_mod)
                right_expr_mod = right_expr_denom - right_expr_mod;
            else
                right_expr_div++;
            bool left_expr_odd = (left_expr_div % 2 == 1);
            left_expr_div >>= 1;
            if (left_expr_div != right_expr_div) {
                comparison_result = (left_expr_div > right_expr_div);
            } else {
                ull temp_right = right_expr_denom - right_expr_mod;
                if (temp_right > right_expr_mod) {
                    if (left_expr_odd)
                        comparison_result = true;
                    else
                        right_expr_mod <<= 1;
                } else {
                    if (!left_expr_odd)
                        comparison_result = false;
                    else
                        right_expr_mod = right_expr_mod - temp_right;
                }
                left_expr_div = left_expr_mod / dif_x_rob;
                right_expr_div = right_expr_mod / a_rob;
                if (left_expr_div != right_expr_div)
                    comparison_result = (left_expr_div > right_expr_div);
                else {
                    left_expr_mod = left_expr_mod % dif_x_rob;
                    right_expr_mod = right_expr_mod % a_rob;
                    comparison_result = (left_expr_mod * a_rob > right_expr_mod * dif_x_rob);
                }
            }
        }
        
        if (segment_site.is_inverse() ^ comparison_result ^ reverse_order)
            return reverse_order ? LESS : MORE;
        return UNDEFINED;
    }

#ifdef USE_MULTI_PRECISION_LIBRARY
    template<typename T>
    bool mpz_less_predicate(point_2d<T> segment_start, point_2d<T> segment_end,
                            point_2d<T> site_point, point_2d<T> new_point, bool reverse_order) {
        mpz_class segment_start_x, segment_start_y, segment_end_x, segment_end_y,
                  site_point_x, site_point_y, new_point_x, new_point_y;
        segment_start_x = static_cast<int>(segment_start.x());
        segment_start_y = static_cast<int>(segment_start.y());
        segment_end_x = static_cast<int>(segment_end.x());
        segment_end_y = static_cast<int>(segment_end.y());
        site_point_x = static_cast<int>(site_point.x());
        site_point_y = static_cast<int>(site_point.y());
        new_point_x = static_cast<int>(new_point.x());
        new_point_y = static_cast<int>(new_point.y());

        mpz_class dif_x, dif_y, a, b, mul1, mul2, temp, left_expr, right_expr;
        dif_x = new_point_x - site_point_x;
        dif_y = new_point_y - site_point_y;
        a = segment_end_x - segment_start_x;
        b = segment_end_y - segment_start_y;
        mul1 = new_point_x - segment_end_x;
        mul2 = new_point_y - segment_end_y;
        temp = dif_x * dif_x + dif_y * dif_y;
        left_expr = (a * a + b * b) * temp * temp;
        right_expr = (2.0 * dif_x * (b * mul1 - a * mul2) - b * temp);
        right_expr = right_expr * right_expr;

        return (!reverse_order) ? (left_expr > right_expr) : (left_expr < right_expr);
    }
#endif

    // Returns true if horizontal line going through new site intersects
    // right arc at first, else returns false. If horizontal line goes
    // through intersection point of the given two arcs returns false also. 
    // Used during nodes comparison.
    // If reverse order is false we are comparing (point, segment) intersection
    // point and new point, else (segment, point) intersection point.
    // (point, segment) and (segment, point) are two distinct points, except
    // case of vertical segment.
    template <typename T>
    bool less_predicate(point_2d<T> site_point, site_event<T> segment_site,
                        point_2d<T> new_point, bool reverse_order) {
        kPredicateResult fast_res = fast_less_predicate(
            site_point, segment_site, new_point, reverse_order);
        if (fast_res != UNDEFINED) {
            return (fast_res == LESS);
        }

        const point_2d<T> &segment_start = segment_site.get_point0();
        const point_2d<T> &segment_end = segment_site.get_point1();
        double dif_x = static_cast<double>(new_point.x()) -
                       static_cast<double>(site_point.x());
        double dif_y = static_cast<double>(new_point.y()) -
                       static_cast<double>(site_point.y());
        double a = static_cast<double>(segment_end.x()) -
                   static_cast<double>(segment_start.x());
        double b = static_cast<double>(segment_end.y()) -
                   static_cast<double>(segment_start.y());

        double mul1 = static_cast<double>(new_point.x()) - static_cast<double>(segment_end.x());
        double mul2 = static_cast<double>(new_point.y()) - static_cast<double>(segment_end.y());
        double temp = dif_x * dif_x + dif_y * dif_y;
        double a_sqr = a * a;
        double b_sqr = b * b;
        double left_expr = (a_sqr * temp + (4.0 * dif_x * mul1) * b_sqr) * temp;
        double right_expr = (4.0 * dif_x * dif_x) * ((mul1 * mul1) * b_sqr + (mul2 * mul2) * a_sqr);
        double common_expr = (4.0 * dif_x * a) * (b * mul2);
        INT_PREDICATE_AVOID_CANCELLATION(common_expr * temp, right_expr, left_expr);
        INT_PREDICATE_AVOID_CANCELLATION(common_expr * (2.0 * dif_x * mul1), left_expr, right_expr);

        if (!almost_equal(left_expr, right_expr, 18)) {
            if (!reverse_order)
                return left_expr > right_expr;
            return left_expr < right_expr;
        }

#ifndef USE_MULTI_PRECISION_LIBRARY
        return (!reverse_order) ? (left_expr > right_expr) : (left_expr < right_expr);
#else
        return mpz_less_predicate(segment_start, segment_end, site_point,
                                  new_point, reverse_order);
#endif
    }

    template <typename T>
    bool less_predicate(site_event<T> left_site, site_event<T> right_site, point_2d<T> new_point) {
        if (left_site.get_site_index() == right_site.get_site_index()) {
            return orientation_test(left_site.get_point0(), left_site.get_point1(),
                new_point) == LEFT_ORIENTATION;
        }

        const point_2d<T> segment1_start = left_site.get_point1();
        const point_2d<T> segment1_end = left_site.get_point0();
        const point_2d<T> segment2_start = right_site.get_point1();
        const point_2d<T> segment2_end = right_site.get_point0();
        double intersection_x1 = 0.0;
        double intersection_x2 = 0.0;
        
        double a1 = static_cast<double>(segment1_end.x()) -
                    static_cast<double>(segment1_start.x());
        if (a1 == 0.0) {
            // Avoid cancellation.
            intersection_x2 += (static_cast<double>(new_point.x()) -
                                static_cast<double>(segment1_end.x())) * 0.5;
        } else {
            double b1 = static_cast<double>(segment1_end.y()) -
                        static_cast<double>(segment1_start.y());
            double c1 = b1 * (static_cast<double>(new_point.x()) -
                              static_cast<double>(segment1_start.x())) +
                        a1 * segment1_start.y();
            double mul1 = sqrt(a1 * a1 + b1 * b1);
            if (left_site.is_inverse()) {
                if (b1 >= 0.0) {
                    mul1 = 1.0 / (b1 + mul1);
                } else {
                    mul1 = (-b1 + mul1) / (a1 * a1);
                }
            } else {
                if (b1 >= 0.0) {
                    mul1 = (-b1 - mul1) / (a1 * a1);
                } else {
                    mul1 = 1.0 / (b1 - mul1);
                }
            }
            INT_PREDICATE_AVOID_CANCELLATION(a1 * mul1 * static_cast<double>(new_point.y()),
                                             intersection_x1, intersection_x2);
            INT_PREDICATE_AVOID_CANCELLATION(-c1 * mul1, intersection_x1, intersection_x2);
        }

        double a2 = static_cast<double>(segment2_end.x()) -
                    static_cast<double>(segment2_start.x());
        if (a2 == 0.0) {
            // Avoid cancellation.
            intersection_x1 += (static_cast<double>(new_point.x()) - 
                                static_cast<double>(segment2_end.x())) * 0.5;
        } else {
            double b2 = static_cast<double>(segment2_end.y()) -
                        static_cast<double>(segment2_start.y());
            double c2 = b2 * (static_cast<double>(new_point.x()) -
                              static_cast<double>(segment2_start.x())) +
                        a2 * segment2_start.y();
            double mul2 = sqrt(a2 * a2 + b2 * b2);
            if (right_site.is_inverse()) {
                if (b2 >= 0.0) {
                    mul2 = 1.0 / (b2 + mul2);
                } else {
                    mul2 = (-b2 + mul2) / (a2 * a2);
                }
            } else {
                if (b2 >= 0.0) {
                    mul2 = (-b2 - mul2) / (a2 * a2);
                } else {
                    mul2 = 1.0 / (b2 - mul2);
                }
            }
            INT_PREDICATE_AVOID_CANCELLATION(a2 * static_cast<double>(new_point.y()) * mul2,
                                             intersection_x2, intersection_x1);
            INT_PREDICATE_AVOID_CANCELLATION(-c2 * mul2, intersection_x2, intersection_x1);
        }

        if (!almost_equal(intersection_x1, intersection_x2, 20)) {
            return intersection_x1 < intersection_x2;
        }
        
        // TODO(asydorchuk): Add mpl support there.
        return intersection_x1 < intersection_x2;
    }

    ///////////////////////////////////////////////////////////////////////////
    // CIRCLE EVENTS CREATION /////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Create circle event from three point sites.
    // TODO (asydorchuk): make precision optimizations.
    template <typename T>
    static bool create_circle_event_ppp(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        circle_event<T> &c_event) {
        double dif_x1 = static_cast<double>(site1.x()) -
                        static_cast<double>(site2.x());
        double dif_x2 = static_cast<double>(site2.x()) -
                        static_cast<double>(site3.x());
        double dif_y1 = static_cast<double>(site1.y()) -
                        static_cast<double>(site2.y());
        double dif_y2 = static_cast<double>(site2.y()) -
                        static_cast<double>(site3.y());
        kOrientation valid_orientation = orientation_test(
            static_cast<long long>(dif_x1), static_cast<long long>(dif_y1),
            static_cast<long long>(dif_x2), static_cast<long long>(dif_y2));
        if (valid_orientation != RIGHT_ORIENTATION)
            return false;
        double a = 2.0 * (dif_x1 * dif_y2 - dif_x2 * dif_y1);
        double b1 = dif_x1 * (site1.x() + site2.x()) + dif_y1 * (site1.y() + site2.y());
        double b2 = dif_x2 * (site2.x() + site3.x()) + dif_y2 * (site2.y() + site3.y());

        // Create new circle event.
        double c_x = (b1*dif_y2 - b2*dif_y1) / a;
        double c_y = (b2*dif_x1 - b1*dif_x2) / a;
        double radius = sqrt((c_x-site2.x())*(c_x-site2.x()) +
                             (c_y-site2.y())*(c_y-site2.y()));
        c_event = make_circle_event<double>(c_x, c_y, c_x + radius);
        return true;
    }

    // Create circle event from two point sites and one segment site.
    // TODO (asydorchuk): make_precision optimizations.
    template <typename T>
    static bool create_circle_event_pps(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        int segment_index,
                                        circle_event<T> &c_event) {
        if (segment_index != 2) {
            if (orientation_test(site3.get_point0(), site1.get_point0(),
                site2.get_point0()) != RIGHT_ORIENTATION &&
                detail::orientation_test(site3.get_point1(), site1.get_point0(),
                site2.get_point0()) != RIGHT_ORIENTATION)
                return false;
        } else {
            if (orientation_test(site1.get_point0(), site3.get_point0(),
                site2.get_point0()) != RIGHT_ORIENTATION &&
                detail::orientation_test(site1.get_point0(), site3.get_point1(),
                site2.get_point0()) != RIGHT_ORIENTATION)
                return false;
        }

        double line_a = site3.get_point1().y() - site3.get_point0().y();
        double line_b = site3.get_point0().x() - site3.get_point1().x();
        double vec_x = site2.y() - site1.y();
        double vec_y = site1.x() - site2.x();
        double teta = line_b * vec_y + line_a * vec_x;
        double A = line_a * (site1.x() - site3.get_point1().x()) +
                   line_b * (site1.y() - site3.get_point1().y());
        double B = line_a * (site2.x() - site3.get_point1().x()) +
                   line_b * (site2.y() - site3.get_point1().y());
        double denom = line_b * vec_x - line_a * vec_y;
        double det = sqrt((teta * teta + denom * denom) * A * B);
        double t;
        
        if (orientation_test(static_cast<long long>(line_a),
                             static_cast<long long>(line_b),
                             static_cast<long long>(vec_x),
                             static_cast<long long>(vec_y)) == COLINEAR) {
            t = (teta * teta - 4.0 * A * B) / (4.0 * teta * (A + B));
        } else {
            if (segment_index == 2)
                det = -det;
            t = (teta * (A + B) + 2.0 * det) / (2.0 * denom * denom);
        }
        double c_x = 0.5 * (site1.x() + site2.x()) + t * vec_x;
        double c_y = 0.5 * (site1.y() + site2.y()) + t * vec_y;
        double radius = sqrt((c_x-site2.x())*(c_x-site2.x()) +
                             (c_y-site2.y())*(c_y-site2.y()));
        c_event = make_circle_event<double>(c_x, c_y, c_x + radius);
        return true;
    }

    // Create circle event from one point site and two segment sites.
    // TODO (asydorchuk): make precision optimizations.
    template <typename T>
    static bool create_circle_event_pss(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        int point_index,
                                        circle_event<T> &c_event) {
        // Intersection check.
        if (site1.get_site_index() == site2.get_site_index()) {
            return false;
        }
        const point_2d<T> &segm_start1 = site2.get_point1(true);
        const point_2d<T> &segm_end1 = site2.get_point0(true);
        const point_2d<T> &segm_start2 = site3.get_point0(true);
        const point_2d<T> &segm_end2 = site3.get_point1(true);

        kOrientation valid_orient1 = orientation_test(segm_end1, segm_start1, segm_start2);
        kOrientation valid_orient2 = orientation_test(segm_end1, segm_start1, segm_end2);
        if (valid_orient1 != RIGHT_ORIENTATION && valid_orient2 != RIGHT_ORIENTATION) {
            return false;
        }

        double a1 = static_cast<double>(segm_end1.x() - segm_start1.x());
        double b1 = static_cast<double>(segm_end1.y() - segm_start1.y());
        double a2 = static_cast<double>(segm_end2.x() - segm_start2.x());
        double b2 = static_cast<double>(segm_end2.y() - segm_start2.y());

        kOrientation segm_orient = orientation_test(
            static_cast<long long>(a1), static_cast<long long>(b1),
            static_cast<long long>(a2), static_cast<long long>(b2));

        if (segm_orient == COLINEAR) {
            double a = a1 * a1 + b1 * b1;
            double b = a1 * ((static_cast<double>(segm_start1.x()) +
                              static_cast<double>(segm_start2.x())) * 0.5 -
                              static_cast<double>(site1.x())) +
                       b1 * ((static_cast<double>(segm_start1.y()) +
                              static_cast<double>(segm_start2.y())) * 0.5 -
                              static_cast<double>(site1.y()));
            double c = b1 * (static_cast<double>(segm_start2.x()) -
                             static_cast<double>(segm_start1.x())) -
                       a1 * (static_cast<double>(segm_start2.y()) -
                             static_cast<double>(segm_start1.y()));
            double det = -(b1 * (static_cast<double>(site1.x()) -
                                 static_cast<double>(segm_start1.x())) -
                           a1 * (static_cast<double>(site1.y()) -
                                 static_cast<double>(segm_start1.y()))) *
                          (b1 * (static_cast<double>(site1.x()) -
                                 static_cast<double>(segm_start2.x())) -
                           a1 * (static_cast<double>(site1.y()) -
                                 static_cast<double>(segm_start2.y())));
            double t = ((point_index == 2) ? (-b + sqrt(det)) : (-b - sqrt(det))) / a;
            double c_x = 0.5 * (static_cast<double>(segm_start1.x() + segm_start2.x())) + a1 * t;
            double c_y = 0.5 * (static_cast<double>(segm_start1.y() + segm_start2.y())) + b1 * t;
            double radius = 0.5 * fabs(c) / sqrt(a);
            c_event = make_circle_event<double>(c_x, c_y, c_x + radius);
            return true;
        } else {
            double c1 = b1 * static_cast<double>(segm_end1.x()) -
                a1 * static_cast<double>(segm_end1.y());
            double c2 = a2 * static_cast<double>(segm_end2.y()) -
                b2 * static_cast<double>(segm_end2.x());
            double denom = (b1 * a2 - b2 * a1);
            double intersection_x = (c1 * a2 + a1 * c2) / denom;
            double intersection_y = (b1 * c2 + b2 * c1) / denom;
            double sqr_sum1 = sqrt(a1 * a1 + b1 * b1);
            double sqr_sum2 = sqrt(a2 * a2 + b2 * b2);
            double vec_x = a1 * sqr_sum2 + a2 * sqr_sum1;
            double vec_y = b1 * sqr_sum2 + b2 * sqr_sum1;
            double dx = intersection_x - site1.get_point0().x();
            double dy = intersection_y - site1.get_point0().y();
            double a = a1 * a2 + b1 * b2 + sqr_sum1 * sqr_sum2;
            double b = dx * vec_x + dy * vec_y;
            double det = -2.0 * a * (b1 * dx - a1 * dy) * (b2 * dx - a2 * dy);
            double t = ((point_index == 2) ? (-b + sqrt(det)) : (-b - sqrt(det))) / (a * a);
            double c_x = intersection_x + vec_x * t;
            double c_y = intersection_y + vec_y * t;
            double radius = fabs(denom * t);
            c_event = make_circle_event<double>(c_x, c_y, c_x + radius);
            return true;
        }
    }

    // Create circle event from three segment sites.
    template <typename T>
    static bool create_circle_event_sss(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        circle_event<T> &c_event) {
        if (site1.get_site_index() == site2.get_site_index() ||
            site2.get_site_index() == site3.get_site_index()) {
            return false;
        }
        double a1 = static_cast<double>(site1.x1(true) - site1.x0(true));
        double b1 = static_cast<double>(site1.y1(true) - site1.y0(true));
        double c1 = b1 * static_cast<double>(site1.x0(true)) -
                    a1 * static_cast<double>(site1.y0(true));
        double a2 = static_cast<double>(site2.x1(true) - site2.x0(true));
        double b2 = static_cast<double>(site2.y1(true) - site2.y0(true));
        double c2 = b2 * static_cast<double>(site2.x0(true)) -
                    a2 * static_cast<double>(site2.y0(true));
        double a3 = static_cast<double>(site3.x1(true) - site3.x0(true));
        double b3 = static_cast<double>(site3.y1(true) - site3.y0(true));
        double c3 = b3 * static_cast<double>(site3.x0(true)) -
                    a3 * static_cast<double>(site3.y0(true));
        double sqr_sum1 = sqrt(a1 * a1 + b1 * b1);
        double sqr_sum2 = sqrt(a2 * a2 + b2 * b2);
        double sqr_sum3 = sqrt(a3 * a3 + b3 * b3);
        double vec_a1 = -a1 * sqr_sum2 + a2 * sqr_sum1;
        double vec_b1 = -b1 * sqr_sum2 + b2 * sqr_sum1;
        double vec_c1 = -c1 * sqr_sum2 + c2 * sqr_sum1;
        double vec_a2 = -a2 * sqr_sum3 + a3 * sqr_sum2;
        double vec_b2 = -b2 * sqr_sum3 + b3 * sqr_sum2;
        double vec_c2 = -c2 * sqr_sum3 + c3 * sqr_sum2;
        double det = vec_a1 * vec_b2 - vec_b1 * vec_a2;
        double c_x = (vec_a1 * vec_c2 - vec_c1 * vec_a2) / det;
        double c_y = (vec_b1 * vec_c2 - vec_c1 * vec_b2) / det;
        double radius = fabs(-b2 * c_x + a2 * c_y + c2) / sqr_sum2;
        c_event = make_circle_event<double>(c_x, c_y, c_x + radius);
        return true;
    }

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

        void set_left_site_inverse() {
            left_site_.set_inverse();
        }

        void set_right_site_inverse() {
            right_site_.set_inverse();
        }

        coordinate_type get_comparison_x() const {
            return (left_site_.get_site_index() >= right_site_.get_site_index()) ?
                   left_site_.x() : right_site_.x();
        }

        std::pair<coordinate_type, int> get_comparison_y(bool is_new_node = true) const {
            if (left_site_.get_site_index() == right_site_.get_site_index()) {
                return std::make_pair(left_site_.y(), 0);
            }
            if (left_site_.get_site_index() > right_site_.get_site_index()) {
                if (!is_new_node && left_site_.is_segment() && left_site_.is_vertical()) {
                    return std::make_pair(left_site_.y1(), 1);
                }
                return std::make_pair(left_site_.y(), 1);
            }
            return std::make_pair(right_site_.y(), -1);
        }

        int get_comparison_index() const {
            return (left_site_.get_site_index() > right_site_.get_site_index()) ?
                   left_site_.get_site_index() : right_site_.get_site_index();
        }

        const site_event_type &get_comparison_site() const {
            return (left_site_.get_site_index() >= right_site_.get_site_index()) ?
                   left_site_ : right_site_;
        }

        bool less(const Point2D &new_site) const {
            if (!left_site_.is_segment()) {
                return (!right_site_.is_segment()) ? less_pp(new_site) : less_ps(new_site);
            } else {
                return (!right_site_.is_segment()) ? less_sp(new_site) : less_ss(new_site);
            }
        }

        bool less_pp(const Point2D &new_site) const {
            if (left_site_.x() > right_site_.x()) {
                if (new_site.y() <= left_site_.y())
                    return false;
                return less_predicate(left_site_.get_point0(), right_site_.get_point0(), new_site);
            } else if (left_site_.x() < right_site_.x()) {
                if (new_site.y() >= right_site_.y())
                    return true;
                return less_predicate(left_site_.get_point0(), right_site_.get_point0(), new_site);
            } else {
                return left_site_.y() + right_site_.y() <
                       static_cast<coordinate_type>(2.0) * new_site.y();
            }
        }

        bool less_ps(const Point2D &new_site) const {
            return less_predicate(left_site_.get_point0(), right_site_, new_site, false);
        }

        bool less_sp(const Point2D &new_site) const {
            return less_predicate(right_site_.get_point0(), left_site_, new_site, true);
        }

        bool less_ss(const Point2D &new_site) const {
            return less_predicate(left_site_, right_site_, new_site);
        }

    private:
        site_event_type left_site_;
        site_event_type right_site_;
    };

    template <typename T>
    struct voronoi_edge;

    // Represents edge data sturcture (bisector segment), which is
    // associated with beach line node key in the binary search tree.
    template <typename T>
    struct beach_line_node_data {
        voronoi_edge<T> *edge;
        typename circle_events_queue<T>::circle_event_type_it circle_event_it;
        bool contains_circle_event;

        explicit beach_line_node_data(voronoi_edge<T> *new_edge) :
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
        // Nodes with less y coordinate of the intersection point go first.
        // Comparison is only called during site events processing. That's why
        // one of the nodes will always lie on the sweepline. Comparison won't
        // work fine for nodes situated above sweepline.
        bool operator() (const BeachLineNode &node1,
                         const BeachLineNode &node2) const {
            // Get x coordinate of the righmost site from both nodes.
            coordinate_type node1_x = node1.get_comparison_x();
            coordinate_type node2_x = node2.get_comparison_x();

            if (node1_x < node2_x) {
                return node1.less(node2.get_comparison_site().get_point0());
            } else if (node1_x > node2_x) {
                return !node2.less(node1.get_comparison_site().get_point0());
            } else {
                if (node1.get_comparison_index() == node2.get_comparison_index()) {
                    return node1.get_comparison_y() < node2.get_comparison_y();
                } else if (node1.get_comparison_index() < node2.get_comparison_index()) {
                    std::pair<coordinate_type, int> y1 = node1.get_comparison_y(false);
                    std::pair<coordinate_type, int> y2 = node2.get_comparison_y();
                    if (y1.first != y2.first) {
                        return y1.first < y2.first;
                    }
                    return (!node1.get_comparison_site().is_segment()) ? (y1.second < 0) : false;
                } else {
                    std::pair<coordinate_type, int> y1 = node1.get_comparison_y();
                    std::pair<coordinate_type, int> y2 = node2.get_comparison_y(false);
                    if (y1.first != y2.first) {
                        return y1.first < y2.first;
                    }
                    return (!node2.get_comparison_site().is_segment()) ? (y2.second > 0) : true;
                }
            }
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT /////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
	template <typename T>
	struct voronoi_cell {
		typedef T coordinate_type;
		typedef site_event<T> site_event_type;

		voronoi_edge<coordinate_type> *incident_edge;
		site_event_type site;
		int num_incident_edges;

		voronoi_cell(const site_event_type &new_site, voronoi_edge<coordinate_type>* edge) :
			incident_edge(edge),
			site(new_site),
			num_incident_edges(0) {}
	};

	template <typename T>
	struct voronoi_vertex {
		typedef T coordinate_type;
		typedef point_2d<T> Point2D;

		voronoi_edge<coordinate_type> *incident_edge;
		Point2D vertex;
		int num_incident_edges;
	    typename std::list< voronoi_vertex<coordinate_type> >::iterator voronoi_record_it;

		voronoi_vertex(const Point2D &point, voronoi_edge<coordinate_type>* edge) :
			incident_edge(edge),
			vertex(point),
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
    struct voronoi_edge {
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
		typedef voronoi_cell<coordinate_type> voronoi_cell_type;
		typedef voronoi_vertex<coordinate_type> voronoi_vertex_type;
        typedef voronoi_edge<coordinate_type> voronoi_edge_type;
        typedef voronoi_edge_clipped<coordinate_type> voronoi_edge_clipped_type;

        voronoi_cell_type *cell;
        voronoi_vertex_type *start_point;
        voronoi_vertex_type *end_point;
        voronoi_edge_type *prev;
        voronoi_edge_type *next;
        voronoi_edge_type *rot_prev;
        voronoi_edge_type *rot_next;
        voronoi_edge_type *twin;
        voronoi_edge_clipped_type *clipped_edge;

        voronoi_edge() :
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
        typedef site_event<coordinate_type> site_event_type;
        typedef circle_event<coordinate_type> circle_event_type;

		typedef voronoi_cell<coordinate_type> voronoi_cell_type;
		typedef std::list<voronoi_cell_type> voronoi_cells_type;
		typedef typename voronoi_cells_type::iterator voronoi_cell_iterator_type;
		typedef typename voronoi_cells_type::const_iterator voronoi_cell_const_iterator_type;

		typedef voronoi_vertex<coordinate_type> voronoi_vertex_type;
		typedef std::list<voronoi_vertex_type> voronoi_vertices_type;
		typedef typename voronoi_vertices_type::iterator voronoi_vertex_iterator_type;
		typedef typename voronoi_vertices_type::const_iterator voronoi_vertex_const_iterator_type;

        typedef voronoi_edge<coordinate_type> edge_type;
        typedef voronoi_edge_clipped<coordinate_type> clipped_edge_type;
        typedef std::list<edge_type> voronoi_edges_type;
        typedef typename voronoi_edges_type::iterator edges_iterator_type;

        typedef voronoi_cell_clipped<coordinate_type> clipped_voronoi_cell_type;
		typedef voronoi_vertex_clipped<coordinate_type> clipped_voronoi_vertex_type;
		typedef voronoi_edge_clipped<coordinate_type> clipped_voronoi_edge_type;

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
            voronoi_rect_ = BRect<coordinate_type>(site.get_point0(), site.get_point0());

            // Update cell records.
            cell_records_.push_back(voronoi_cell_type(site, NULL));
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
                    cell_records_.end(), voronoi_cell_type(site1, &edge1)));
                cell_records_.back().num_incident_edges++;
                num_cell_records_++;
                voronoi_rect_ = BRect<coordinate_type>(site1.get_point0(), site1.get_point0());
            }

            // Update bounding rectangle.
			voronoi_rect_.update(site2.get_point0());
			if (site2.is_segment()) {
				voronoi_rect_.update(site2.get_point1());	
			}

            // Second site represents new site during site event processing.
            // Add new cell to the cell records vector.
            cell_iterators_.push_back(cell_records_.insert(
                cell_records_.end(), voronoi_cell_type(site2, &edge2)));
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
            vertex_records_.push_back(voronoi_vertex_type(circle.get_center(), edge12));
            voronoi_vertex_type &new_vertex = vertex_records_.back();
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

        const voronoi_cells_type &get_cell_records() const {
            return cell_records_;
        }

        const voronoi_vertices_type &get_voronoi_vertices() const {
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
				std::advance(edge_it, 2);

                if (edge_it1->start_point && edge_it1->end_point &&
                    edge_it1->start_point->vertex == edge_it1->end_point->vertex) {
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
            for (voronoi_cell_iterator_type cell_it = cell_records_.begin();
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
            offset *= static_cast<coordinate_type>(1.0);

            if (offset == static_cast<coordinate_type>(0.0))
                offset = 0.5;

            BRect<coordinate_type> new_brect(x_mid - offset, y_mid - offset,
											 x_mid + offset, y_mid + offset);
            clip(new_brect, clipped_output);
        }

    private:
        // Simplify degenerate edge.
        void simplify_edge(edge_type *edge) {
            voronoi_vertex_type *vertex1 = edge->start_point;
            voronoi_vertex_type *vertex2 = edge->end_point;

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

		void clip(const BRect<coordinate_type> &brect,
                  voronoi_output_clipped<coordinate_type> &clipped_output) {
            if (!brect.is_valid())
                return;
            clipped_output.reset();
            clipped_output.bounding_rectangle = brect;
            
            // collinear input sites case.
            if (num_vertex_records_ == 0) {
                // Return in case of single site input.
                if (num_cell_records_ == 1) {
					clipped_output.cell_records.push_back(
						clipped_voronoi_cell_type(cell_records_.back().site, NULL));
					clipped_output.num_cell_records++;
					return;
                }

                edges_iterator_type edge_it = edges_.begin();
                while (edge_it != edges_.end()) {
                    edge_type *cur_edge = &(*edge_it);
                    edge_it++;
                    edge_type *cur_twin_edge = &(*edge_it);
                    edge_it++;
                    // Add new clipped edges.
                    clipped_edge_type &new_edge = add_clipped_edge(clipped_output);
                    clipped_edge_type &new_twin_edge = add_clipped_edge(clipped_output);

                    // Update twin pointers.
                    new_edge.twin = &new_twin_edge;
                    new_twin_edge.twin = &new_edge;

                    // Update clipped edge pointers.
                    cur_edge->clipped_edge = &new_edge;
                    cur_twin_edge->clipped_edge = &new_twin_edge;
                }
            } else {
				// Iterate over all voronoi vertices.
				for (voronoi_vertex_const_iterator_type vertex_it = vertex_records_.begin();
					 vertex_it != vertex_records_.end(); vertex_it++) {
					edge_type *cur_edge = vertex_it->incident_edge;
					const Point2D &cur_vertex_point = vertex_it->vertex;

					// Add current voronoi vertex to clipped output.
					clipped_output.vertex_records.push_back(
						clipped_voronoi_vertex_type(cur_vertex_point, NULL));
					clipped_output.num_vertex_records++;
					clipped_voronoi_vertex_type &new_vertex = clipped_output.vertex_records.back();
					new_vertex.num_incident_edges = vertex_it->num_incident_edges;
					clipped_edge_type *rot_prev_edge = NULL;

					// Process all half-edges incident to the current voronoi vertex.
					do {
						// Add new edge to the clipped output.
						clipped_edge_type &new_edge = add_clipped_edge(clipped_output);
						new_edge.start_point = &new_vertex;
						cur_edge->clipped_edge = &new_edge;

						// Ray edges with no start point don't correspond to any voronoi vertex.
						// That's why ray edges are processed during their twin edge processing.
						if (cur_edge->end_point == NULL) {
							// Add new twin edge.
							clipped_edge_type &new_twin_edge = add_clipped_edge(clipped_output);
							cur_edge->twin->clipped_edge = &new_twin_edge;
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
				}
            }

            // Iterate over all voronoi cells.
            for (voronoi_cell_const_iterator_type cell_it = cell_records_.begin();
                 cell_it != cell_records_.end(); cell_it++) {
                // Add new cell to the clipped output.
			    clipped_output.cell_records.push_back(
					clipped_voronoi_cell_type(cell_it->site, NULL));
				clipped_output.num_cell_records++;
                clipped_voronoi_cell_type &new_cell = clipped_output.cell_records.back();
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

				if (new_cell.incident_edge == NULL) {
					clipped_output.cell_records.pop_back();
                    clipped_output.num_cell_records--;
				} else {
                    // Update prev/next pointers.
                    prev->next = new_cell.incident_edge;
                    new_cell.incident_edge->prev = prev;
                }
            }
			clipped_output.num_edge_records >>= 1;
        }

		inline clipped_voronoi_edge_type &add_clipped_edge(
			voronoi_output_clipped<coordinate_type> &clipped_output) {
			clipped_output.edge_records.push_back(clipped_voronoi_edge_type());
			clipped_output.num_edge_records++;
			return clipped_output.edge_records.back();
		}

        std::vector<voronoi_cell_iterator_type> cell_iterators_;
        voronoi_cells_type cell_records_;
        voronoi_vertices_type vertex_records_;
        voronoi_edges_type edges_;

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

#undef INV_LOG_2
#undef INT_PREDICATE_COMPUTE_DIFFERENCE
#undef INT_PREDICATE_CONVERT_65_BIT
#undef INT_PREDICATE_AVOID_CANCELLATION

#endif

// Boost sweepline/voronoi_formation.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_FORMATION
#define BOOST_SWEEPLINE_VORONOI_FORMATION

namespace boost {
namespace sweepline {
namespace detail {

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI EVENT TYPES ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Forward declarations.
    template <typename T>
    class beach_line_node;

    template <typename T>
    class beach_line_node_data;

    template <typename T>
    struct node_comparer;

    template <typename T>
    class epsilon_robust_comparator;

    // Represents the result of the orientation test.
    enum kOrientation {
        RIGHT_ORIENTATION = -1,
        COLLINEAR = 0,
        LEFT_ORIENTATION = 1,
    };

    // Represents the result of the epsilon robust predicate.
    // If the result is undefined some further processing is usually required.
    enum kPredicateResult {
        LESS = -1,
        UNDEFINED = 0,
        MORE = 1,
    };

    // Site event type.
    // Occurs when the sweepline sweeps over one of the initial sites:
    //     1) point site;
    //     2) startpoint of the segment site;
    //     3) endpoint of the segment site.
    // Implicit segment direction is defined: the startpoint of
    // the segment compares less than its endpoint.
    // Each input segment is divided onto two site events:
    //     1) One going from the startpoint to the endpoint
    //        (is_inverse_ = false);
    //     2) Another going from the endpoint to the startpoint
    //        (is_inverse_ = true).
    // In beach line data structure segment sites of the first
    // type preceed sites of the second type for the same segment.
    // Variables: point0_ - point site or segment's startpoint;
    //            point1_ - segment's endpoint if site is a segment;
    //            index_ - site event index among other sites;
    //            is_segment_ - flag whether site is a segment;
    //            is_vertical_ - flag whether site is vertical;
    //            is_inverse_ - defines type of the segment site.
    // Note: for all the point sites is_vertical_ flag is true,
    //       is_inverse_ flag is false.
    template <typename T>
    class site_event {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> point_2d_type;

        // Point site contructors.
        site_event() :
            is_segment_(false),
            is_vertical_(true),
            is_inverse_(false) {}

        site_event(coordinate_type x, coordinate_type y, int index) :
            point0_(x, y),
            site_index_(index),
            is_segment_(false),
            is_vertical_(true),
            is_inverse_(false) {}

        site_event(const point_2d_type &point, int index) :
            point0_(point),
            site_index_(index),
            is_segment_(false),
            is_vertical_(true),
            is_inverse_(false) {}

        // Segment site constructors.
        site_event(coordinate_type x1, coordinate_type y1,
                   coordinate_type x2, coordinate_type y2, int index):
            point0_(x1, y1),
            point1_(x2, y2),
            site_index_(index),
            is_segment_(true),
            is_inverse_(false) {
            if (point0_ > point1_)
                (std::swap)(point0_, point1_);
            is_vertical_ = (point0_.x() == point1_.x());
        }

        site_event(const point_2d_type &point1,
                   const point_2d_type &point2, int index) :
            point0_(point1),
            point1_(point2),
            site_index_(index),
            is_segment_(true),
            is_inverse_(false) {
            if (point0_ > point1_)
                (std::swap)(point0_, point1_);
            is_vertical_ = (point0_.x() == point1_.x());
        }

        bool operator==(const site_event &that) const {
            return (point0_ == that.point0_) &&
                   ((!is_segment_ && !that.is_segment_) ||
                   (is_segment_ && that.is_segment_ &&
                    point1_ == that.point1_));
        }

        bool operator!=(const site_event &that) const {
            return !((*this) == that);
        }

        // All the sites are sorted by the coordinates of the first point.
        // Point sites preceed vertical segment sites with equal first points.
        // Point sites and vertical segments preceed non-vertical segments
        // with the same x-coordinate of the first point (for any y).
        // Non-vertical segments with equal first points are ordered using
        // counterclockwise direction starting from the positive direction of
        // the x-axis. Such ordering simplifies the initialization step of the
        // algorithm.
        bool operator<(const site_event &that) const {
            // Compare x-coordinates of the first points.
            if (this->point0_.x() != that.point0_.x())
                return this->point0_.x() < that.point0_.x();

            // X-coordinates of the first points are the same.
            if (!(this->is_segment_)) {
                // The first site is a point.
                if (!that.is_segment_) {
                    // The second site is a point.
                    return this->point0_.y() < that.point0_.y();
                }
                if (that.is_vertical_) {
                    // The second site is a vertical segment.
                    return this->point0_.y() <= that.point0_.y();
                }
                // The second site is a non-vertical segment.
                return true;
            } else {
                // The first site is a segment.
                if (that.is_vertical_) {
                    // The second site is a point or a vertical segment.
                    if (this->is_vertical_) {
                        // The first site is a vertical segment.
                        return this->point0_.y() < that.point0_.y();
                    }
                    // The first site is a non-vertical segment.
                    return false;
                }

                // The second site is a non-vertical segment.
                if (this->is_vertical_) {
                    // The first site is a vertical segment.
                    return true;
                }

                // Compare y-coordinates of the first points.
                if (this->point0_.y() != that.point0_.y())
                    return this->point0_.y() < that.point0_.y();

                // Y-coordinates of the first points are the same.
                // Both sites are segment. Sort by angle using CCW ordering
                // starting at the positive direction of the x axis.
                return orientation_test(this->point1_, this->point0_,
                                        that.point1_) == LEFT_ORIENTATION;
            }
        }

        bool operator<=(const site_event &that) const {
            return !(that < (*this));
        }

        bool operator>(const site_event &that) const {
            return that < (*this);
        }

        bool operator>=(const site_event &that) const {
            return !((*this) < that);
        }

        coordinate_type x(bool oriented = false) const {
            return x0(oriented);
        }

        coordinate_type y(bool oriented = false) const {
            return y0(oriented);
        }

        // Return x-coordinate of the point for the point sites.
        // Return x-coordinate of the startpoint for the segment sites.
        coordinate_type x0(bool oriented = false) const {
            if (!oriented)
                return point0_.x();
            return is_inverse_ ? point1_.x() : point0_.x();
        }

        // Return y-coordinate of the point for the point sites.
        // Return y-coordinate of the startpoint for the segment sites.
        coordinate_type y0(bool oriented = false) const {
            if (!oriented)
                return point0_.y();
            return is_inverse_ ? point1_.y() : point0_.y();
        }

        // Return x-coordinate of the endpoint of the segment sites.
        // Doesn't make sense for the point sites.
        coordinate_type x1(bool oriented = false) const {
            if (!oriented)
                return point1_.x();
            return is_inverse_ ? point0_.x() : point1_.x();
        }

        // Return y-coordinate of the endpoint of the segment sites.
        // Doesn't make sense for the point sites.
        coordinate_type y1(bool oriented = false) const {
            if (!oriented)
                return point1_.y();
            return is_inverse_ ? point0_.y() : point1_.y();
        }

        // Return point for the point sites.
        // Return startpoint for the segment sites.
        const point_2d_type &point0(bool oriented = false) const {
            if (!oriented)
                return point0_;
            return is_inverse_ ? point1_ : point0_;
        }

        // Return endpoint of the segment sites.
        // Doesn't make sense for the point sites.
        const point_2d_type &point1(bool oriented = false) const {
            if (!oriented)
                return point1_;
            return is_inverse_ ? point0_ : point1_;
        }

        // Return index of the site among all the other sites.
        void index(int index) {
            site_index_ = index;
        }

        // Change segment site orientation to the opposite one.
        void inverse() {
            is_inverse_ ^= true;
        }

        int index() const {
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
        point_2d_type point0_;
        point_2d_type point1_;
        int site_index_;
        bool is_segment_;
        bool is_vertical_;
        bool is_inverse_;
    };

    template <typename T>
    static inline site_event<T> make_site_event(T x, T y, int index) {
        return site_event<T>(x, y, index);
    }

    template <typename T>
    static inline site_event<T> make_site_event(const point_2d<T> &point,
                                                int index) {
        return site_event<T>(point, index);
    }

    template <typename T>
    static inline site_event<T> make_site_event(T x1, T y1, T x2, T y2,
                                                int index) {
        return site_event<T>(x1, y1, x2, y2, index);
    }

    template <typename T>
    static inline site_event<T> make_site_event(const point_2d<T> &point1,
                                                const point_2d<T> &point2,
                                                int index) {
        return site_event<T>(point1, point2, index);
    }

    // Circle event type.
    // Occrus when the sweepline sweeps over the rightmost point of the voronoi
    // circle (with the center at the intersection point of the bisectors).
    // Circle event is made by the two consequtive nodes in the beach line data
    // structure. In case another node was inserted during algorithm execution
    // between the given two nodes circle event becomes inactive.
    // Circle events order is based on the comparison of the rightmost points
    // of the circles. The order is the same like for the point_2d class.
    // However as coordinates of the circle events might be not integers extra
    // comparison checks are required to make the comparison robust.
    // Next representation is used to store parameters of the circle:
    //     each of the parameters is represented as fraction
    //     numerator / denominator. Denominator is common for each of the
    //     three parameters. Epsilon robust comparator class is used
    //     to represent parameters of the circle events.
    // Variables: center_x_ - numerator of the center's x-coordinate.
    //            center_y_ - numerator of the center's y-coordinate.
    //            lower_x_ - numerator of the bottom point's x-coordinate.
    //            denom_ - positive denominator for the previous three values.
    //            bisector_node_ - iterator to the second bisector's node.
    //            is_active_ - flag whether circle event is still active.
    // lower_y coordinate is always equal to center_y.
    template <typename T>
    class circle_event {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> point_2d_type;
        typedef site_event<T> site_event_type;
        typedef epsilon_robust_comparator<T> erc_type;
        typedef beach_line_node<coordinate_type> Key;
        typedef beach_line_node_data<coordinate_type> Value;
        typedef node_comparer<Key> NodeComparer;
        typedef typename std::map< Key, Value, NodeComparer >::iterator
            beach_line_iterator;

        circle_event() : is_active_(true) {}

        circle_event(coordinate_type c_x,
                     coordinate_type c_y,
                     coordinate_type lower_x) :
            center_x_(c_x),
            center_y_(c_y),
            lower_x_(lower_x),
            denom_(1),
            is_active_(true) {}

        circle_event(const erc_type &c_x,
                     const erc_type &c_y,
                     const erc_type &lower_x) :
            center_x_(c_x),
            center_y_(c_y),
            lower_x_(lower_x),
            denom_(1),
            is_active_(true) {}

        circle_event(const erc_type &c_x,
                     const erc_type &c_y,
                     const erc_type &lower_x,
                     const erc_type &denom) :
                center_x_(c_x),
                center_y_(c_y),
                lower_x_(lower_x),
                denom_(denom),
                is_active_(true) {
            if (denom_.abs()) {
                center_x_.swap();
                center_y_.swap();
                lower_x_.swap();
            }
        }

        bool operator==(const circle_event &that) const {
            erc_type lhs1 = lower_x_ * that.denom_;
            erc_type rhs1 = denom_ * that.lower_x_;
            erc_type lhs2 = center_y_ * that.denom_;
            erc_type rhs2 = denom_ * that.center_y_;
            return (lhs1.compare(rhs1, 64) == UNDEFINED &&
                    lhs2.compare(rhs2, 64) == UNDEFINED);
        }

        bool operator!=(const circle_event &that) const {
            return !((*this) == that);
        }

        // Compare rightmost points of the circle events.
        // Each rightmost point has next representation:
        // (lower_x / denom, center_y / denom), denom is always positive.
        bool operator<(const circle_event &that) const {
            // Evaluate comparison expressions for x-coordinates.
            erc_type lhs1 = lower_x_ * that.denom_;
            erc_type rhs1 = denom_ * that.lower_x_;

            // Compare x-coordinates of the rightmost points. If the result
            // is undefined we assume that x-coordinates are equal.
            kPredicateResult pres = lhs1.compare(rhs1, 64);
            if (pres != UNDEFINED)
                return (pres == LESS);

            // Evaluate comparison expressions for y-coordinates.
            erc_type lhs2 = center_y_ * that.denom_;
            erc_type rhs2 = denom_ * that.center_y_;

            // Compare y-coordinates of the rightmost points.
            return (lhs2.compare(rhs2, 64) == LESS);
        }

        bool operator<=(const circle_event &that) const {
            return !(that < (*this));
        }

        bool operator>(const circle_event &that) const {
            return that < (*this);
        }

        bool operator>=(const circle_event &that) const {
            return !((*this) < that);
        }

        // Compare the rightmost point of the circle event with
        // the point that represents the site event.
        // If circle point is less than site point return -1.
        // If circle point is equal to site point return 0.
        // If circle point is greater than site point return 1.
        kPredicateResult compare(const site_event_type &s_event) const {
            // Compare x-coordinates.
            kPredicateResult pres = lower_x_.compare(denom_ * s_event.x(), 64);
            // If the comparison result is undefined
            // x-coordinates are considered to be equal.
            if (pres != UNDEFINED)
                return pres;
            // Compare y-coordinates in case of equal x-coordinates.
            return center_y_.compare(denom_ * s_event.y(), 64);
        }

        // Evaluate x-coordinate of the center of the circle.
        coordinate_type x() const {
            return center_x_.dif() / denom_.dif();
        }

        // Evaluate y-coordinate of the center of the circle.
        coordinate_type y() const {
            return center_y_.dif() / denom_.dif();
        }

        // Evaluate x-coordinate of the rightmost point of the circle.
        coordinate_type lower_x() const {
            return lower_x_.dif() / denom_.dif();
        }

        point_2d_type center() const {
            return make_point_2d(x(), y());
        }

        const erc_type &erc_x() const {
            return center_x_;
        }

        const erc_type &erc_y() const {
            return center_y_;
        }

        const erc_type &erc_denom() const {
            return denom_;
        }

        // Return iterator to the second bisector from the beach line
        // data structure that forms current circle event.
        const beach_line_iterator &bisector() const {
            return bisector_node_;
        }

        void bisector(beach_line_iterator iterator) {
            bisector_node_ = iterator;
        }

        bool is_active() const {
            return is_active_;
        }

        void deactivate() {
            is_active_ = false;
        }

    private:
        erc_type center_x_;
        erc_type center_y_;
        erc_type lower_x_;
        erc_type denom_;
        beach_line_iterator bisector_node_;
        bool is_active_;
    };

    template <typename T>
    static inline circle_event<T> make_circle_event(T c_x, T c_y, T lower_x) {
        return circle_event<T>(c_x, c_y, lower_x);
    }

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI CIRCLE EVENTS QUEUE ////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Event queue data structure, holds circle events.
    // During algorithm run, some of the circle events disappear(become
    // inactive). Priority queue data structure by itself doesn't support
    // iterators (there is no direct ability to modify its elements).
    // Instead list is used to store all the circle events and priority queue
    // of the iterators to the list elements is used to keep the correct circle
    // events ordering.
    template <typename T>
    class circle_events_queue {
    public:
        typedef T coordinate_type;
        typedef circle_event<T> circle_event_type;
        typedef typename std::list<circle_event_type>::iterator
            circle_event_type_it;

        circle_events_queue() {}

        void clear() {
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

        // Remove all the inactive events from the top of the priority
        // queue until the first active event is found.
        void remove_not_active_events() {
            while (!circle_events_.empty() &&
                   !circle_events_.top()->is_active())
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

    // Geometry predicates with floating-point variables usually require
    // high-precision predicates to retrieve the correct result.
    // Epsilon robust predicates give the result within some epsilon relative
    // error, but are a lot faster than high-precision predicates.
    // To make algorithm robust and efficient epsilon robust predicates are
    // used at the first step. In case of the undefined result high-precision
    // arithmetic is used to produce required robustness. This approach
    // requires exact computation of epsilon intervals within which epsilon
    // robust predicates have undefined value.
    // There are two ways to measure an error of floating-point calculations:
    // relative error and ULPs(units in the last place).
    // Let EPS be machine epsilon, then next inequalities have place:
    // 1 EPS <= 1 ULP <= 2 EPS (1), 0.5 ULP <= 1 EPS <= 1 ULP (2).
    // ULPs are good for measuring rounding errors and comparing values.
    // Relative erros are good for computation of general relative
    // errors of formulas or expressions. So to calculate epsilon
    // intervals within which epsilon robust predicates have undefined result
    // next schema is used:
    //     1) Compute rounding errors of initial variables using ULPs;
    //     2) Transform ULPs to epsilons using upper bound of the (1);
    //     3) Compute relative error of the formula using epsilon arithmetic;
    //     4) Transform epsilon to ULPs using upper bound of the (2);
    // In case two values are inside undefined ULP range use high-precision
    // arithmetic to produce the correct result, else output the result.
    // Look at almost_equal function to see how two floating-point variables
    // are checked to fit in the ULP range.
    // If A has relative error of r(A) and B has relative error of r(B) then:
    //     1) r(A + B) <= max(r(A), r(B)), for A * B >= 0;
    //     2) r(A - B) <= B*r(A)+A*r(B)/(A-B), for A * B >= 0;
    //     2) r(A * B) <= r(A) + r(B);
    //     3) r(A / B) <= r(A) + r(B);
    // In addition rounding error should be added, that is always equal to
    // 0.5 ULP or atmost 1 epsilon. As you might see from the above formulas
    // substraction relative error may be extremely large, that's why
    // epsilon robust comparator class is used to store floating point values
    // and avoid substraction.
    // For further information about relative errors and ULPs try this link:
    // http://docs.sun.com/source/806-3568/ncg_goldberg.html

    // Convert value to 64-bit unsigned integer.
    // Return true if the value is positive, else false.
    template <typename T>
    static inline bool convert_to_65_bit(T value, unsigned long long &res) {
        if (value >= 0) {
            res = static_cast<unsigned long long>(value);
            return true;
        } else {
            res = static_cast<unsigned long long>(-value);
            return false;
        }
    }

    // If two floating-point numbers in the same format are ordered (x < y),
    // then they are ordered the same way when their bits are reinterpreted as
    // sign-magnitude integers. Values are considered to be almost equal if
    // their integer reinterpretatoins differ in not more than maxUlps units.
    static inline bool almost_equal(double a, double b, int maxUlps) {
        long long ll_a, ll_b;

        // Reinterpret double bits as 64-bit signed integer.
        memcpy(&ll_a, &a, sizeof(double));
        memcpy(&ll_b, &b, sizeof(double));

        // Positive 0.0 is integer zero. Negative 0.0 is 0x8000000000000000.
        // Map negative zero to an integer zero representation - making it
        // identical to positive zero - the smallest negative number is
        // represented by negative one, and downwards from there.
        if (ll_a < 0)
            ll_a = 0x8000000000000000LL - ll_a;
        if (ll_b < 0)
            ll_b = 0x8000000000000000LL - ll_b;

        // Compare 64-bit signed integer representations of input values.
        // Difference in 1 Ulp is equivalent to a relative error of between
        // 1/4,000,000,000,000,000 and 1/8,000,000,000,000,000.
        long long dif = ll_a - ll_b;
        return (dif <= maxUlps) && (dif >= -maxUlps);
    }

    // Robust orientation test. Works correctly for any input type that
    // can be casted without lose of data to the integer type within a range
    // [-2^32, 2^32-1].
    // Arguments: dif_x1_, dif_y1 - coordinates of the first vector.
    //            dif_x2_, dif_y2 - coordinates of the second vector.
    // Returns orientation test result for input vectors.
    template <typename T>
    static kOrientation orientation_test(T dif_x1_, T dif_y1_,
                                         T dif_x2_, T dif_y2_) {
        typedef unsigned long long ull;
        ull dif_x1, dif_y1, dif_x2, dif_y2;
        bool dif_x1_plus, dif_x2_plus, dif_y1_plus, dif_y2_plus;
        dif_x1_plus = convert_to_65_bit(dif_x1_, dif_x1);
        dif_y1_plus = convert_to_65_bit(dif_y1_, dif_y1);
        dif_x2_plus = convert_to_65_bit(dif_x2_, dif_x2);
        dif_y2_plus = convert_to_65_bit(dif_y2_, dif_y2);

        ull expr_l = dif_x1 * dif_y2;
        ull expr_r = dif_x2 * dif_y1;

        bool expr_l_plus = (dif_x1_plus == dif_y2_plus) ? true : false;
        bool expr_r_plus = (dif_x2_plus == dif_y1_plus) ? true : false;

        if (expr_l == 0)
            expr_l_plus = true;
        if (expr_r == 0)
            expr_r_plus = true;

        if ((expr_l_plus == expr_r_plus) && (expr_l == expr_r))
            return COLLINEAR;

        if (!expr_l_plus) {
            if (expr_r_plus)
                return RIGHT_ORIENTATION;
            else
                return (expr_l > expr_r) ?
                       RIGHT_ORIENTATION : LEFT_ORIENTATION;
        } else {
            if (!expr_r_plus)
                return LEFT_ORIENTATION;
            else
                return (expr_l < expr_r) ?
                       RIGHT_ORIENTATION : LEFT_ORIENTATION;
        }
    }

    // Robust orientation test. Works correctly for any input coordinate type
    // that can be casted without lose of data to integer type within a range
    // [-2^31, 2^31 - 1] - signed integer type.
    // Arguments: point1, point2 - represent the first vector;
    //            point2, point3 - represent the second vector;
    // Returns orientation test result for input vectors.
    template <typename T>
    static inline kOrientation orientation_test(const point_2d<T> &point1,
                                                const point_2d<T> &point2,
                                                const point_2d<T> &point3) {
        return orientation_test(point1.x() - point2.x(),
                                point1.y() - point2.y(),
                                point2.x() - point3.x(),
                                point2.y() - point3.y());
    }

    // Value is a determinant of two vectors.
    // Return orientation based on the sign of the determinant.
    template <typename T>
    static inline kOrientation orientation_test(T value) {
        if (value == static_cast<T>(0.0))
            return COLLINEAR;
        return (value < static_cast<T>(0.0)) ?
               RIGHT_ORIENTATION : LEFT_ORIENTATION;
    }

    // Compute robust cross_product: a1 * b2 - b1 * a2.
    // The result is correct with epsilon relative error equal to 1EPS.
    template <typename T>
    static double robust_cross_product(T a1_, T b1_, T a2_, T b2_) {
        typedef unsigned long long ull;
        ull a1, b1, a2, b2;
        bool a1_plus, a2_plus, b1_plus, b2_plus;
        a1_plus = convert_to_65_bit(a1_, a1);
        b1_plus = convert_to_65_bit(b1_, b1);
        a2_plus = convert_to_65_bit(a2_, a2);
        b2_plus = convert_to_65_bit(b2_, b2);

        ull expr_l = a1 * b2;
        bool expr_l_plus = (a1_plus == b2_plus) ? true : false;
        ull expr_r = b1 * a2;
        bool expr_r_plus = (a2_plus == b1_plus) ? true : false;

        if (expr_l == 0)
            expr_l_plus = true;
        if (expr_r == 0)
            expr_r_plus = true;

        if ((expr_l_plus == expr_r_plus) && (expr_l == expr_r))
            return static_cast<T>(0.0);

        // Produce the result with epsilon relative error.
        if (!expr_l_plus) {
            if (expr_r_plus)
                return -static_cast<double>(expr_l) -
                        static_cast<double>(expr_r);
            else return (expr_l > expr_r) ?
                        -static_cast<double>(expr_l - expr_r) :
                        static_cast<double>(expr_r - expr_l);
        } else {
            if (!expr_r_plus)
                return static_cast<double>(expr_l) +
                       static_cast<double>(expr_r);
            else
                return (expr_l < expr_r) ?
                       -static_cast<double>(expr_r - expr_l) :
                       static_cast<double>(expr_l - expr_r);
        }
    }

    // Class used to make computations with epsilon relative error.
    // ERC consists of two values: value1 and value2, both positive.
    // The resulting expression is equal to the value1 - value2.
    // The main idea is to represent any expression that consists of
    // addition, substraction, multiplication and division operations
    // to avoid using substraction. Substraction of a positive value
    // is equivalent to the addition to value2 and substraction of
    // a negative value is equivalent to the addition to value1.
    // Cons: ERC gives error relative not to the resulting value,
    //       but relative to some expression instead. Example:
    //       center_x = 100, ERC's value1 = 10^20, value2 = 10^20,
    //       center_x = 1000, ERC's value3 = 10^21, value4 = 10^21,
    //       such two centers are considered equal(
    //       value1 + value4 = value2 + value3), while they are not.
    // Pros: ERC is much faster then approaches based on the use
    //       of high-precision libraries. However this will give correct
    //       answer for the previous example.
    // Solution: Use ERCs in case of defined comparison results and use
    //           high-precision libraries for undefined results.
    template <typename T>
    class epsilon_robust_comparator {
    public:
        epsilon_robust_comparator() :
          positive_sum_(0),
          negative_sum_(0) {}

        epsilon_robust_comparator(T value) :
          positive_sum_((value>0)?value:0),
          negative_sum_((value<0)?-value:0) {}

        epsilon_robust_comparator(T pos, T neg) :
          positive_sum_(pos),
          negative_sum_(neg) {}

        T dif() const {
            return positive_sum_ - negative_sum_;
        }

        T pos() const {
            return positive_sum_;
        }

        T neg() const {
            return negative_sum_;
        }

        // Equivalent to the unary minus.
        void swap() {
            (std::swap)(positive_sum_, negative_sum_);
        }

        bool abs() {
            if (positive_sum_ < negative_sum_) {
                swap();
                return true;
            }
            return false;
        }

        epsilon_robust_comparator<T> &operator+=(const T &val) {
            if (val >= 0)
                positive_sum_ += val;
            else
                negative_sum_ -= val;
            return *this;
        }

        epsilon_robust_comparator<T> &operator+=(
            const epsilon_robust_comparator<T> &erc) {
            positive_sum_ += erc.positive_sum_;
            negative_sum_ += erc.negative_sum_;
            return *this;
        }

        epsilon_robust_comparator<T> &operator-=(const T &val) {
            if (val >= 0)
                negative_sum_ += val;
            else
                positive_sum_ -= val;
            return *this;
        }

        epsilon_robust_comparator<T> &operator-=(
            const epsilon_robust_comparator<T> &erc) {
            positive_sum_ += erc.negative_sum_;
            negative_sum_ += erc.positive_sum_;
            return *this;
        }

        epsilon_robust_comparator<T> &operator*=(const T &val) {
            positive_sum_ *= fabs(val);
            negative_sum_ *= fabs(val);
            if (val < 0) {
                swap();
            }
            return *this;
        }

        epsilon_robust_comparator<T> &operator/=(const T &val) {
            positive_sum_ /= fabs(val);
            negative_sum_ /= fabs(val);
            if (val < 0) {
                swap();
            }
            return *this;
        }

        // Compare predicate with value using ulp precision.
        kPredicateResult compare(T value, int ulp = 0) const {
            T lhs = positive_sum_ - ((value < 0) ? value : 0);
            T rhs = negative_sum_ + ((value > 0) ? value : 0);
            if (almost_equal(lhs, rhs, ulp))
                return UNDEFINED;
            return (lhs < rhs) ? LESS : MORE;
        }

        // Compare two predicats using ulp precision.
        kPredicateResult compare(const epsilon_robust_comparator &rc,
                                 int ulp = 0) const {
            T lhs = positive_sum_ + rc.neg();
            T rhs = negative_sum_ + rc.pos();
            if (almost_equal(lhs, rhs, ulp))
                return UNDEFINED;
            return (lhs < rhs) ? LESS : MORE;
        }

        // Check whether comparison has undefined result.
        bool compares_undefined(const epsilon_robust_comparator &rc,
                                int ulp) const {
            return this->compare(rc, ulp) == UNDEFINED;
        }

    private:
        T positive_sum_;
        T negative_sum_;
    };

    template<typename T>
    inline epsilon_robust_comparator<T> operator+(
        const epsilon_robust_comparator<T>& lhs,
        const epsilon_robust_comparator<T>& rhs) {
        return epsilon_robust_comparator<T>(lhs.pos() + rhs.pos(),
                                            lhs.neg() + rhs.neg());
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator-(
        const epsilon_robust_comparator<T>& lhs,
        const epsilon_robust_comparator<T>& rhs) {
        return epsilon_robust_comparator<T>(lhs.pos() - rhs.neg(),
                                            lhs.neg() + rhs.pos());
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator*(
        const epsilon_robust_comparator<T>& lhs,
        const epsilon_robust_comparator<T>& rhs) {
        double res_pos = lhs.pos() * rhs.pos() + lhs.neg() * rhs.neg();
        double res_neg = lhs.pos() * rhs.neg() + lhs.neg() * rhs.pos();
        return epsilon_robust_comparator<T>(res_pos, res_neg);
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator*(
        const epsilon_robust_comparator<T>& lhs, const T& val) {
        if (val >= 0)
            return epsilon_robust_comparator<T>(lhs.pos() * val,
                                                lhs.neg() * val);
        return epsilon_robust_comparator<T>(-lhs.neg() * val,
                                            -lhs.pos() * val);
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator*(
        const T& val, const epsilon_robust_comparator<T>& rhs) {
        if (val >= 0)
            return epsilon_robust_comparator<T>(val * rhs.pos(),
                                                val * rhs.neg());
        return epsilon_robust_comparator<T>(-val * rhs.neg(),
                                            -val * rhs.pos());
    }

    // Robust voronoi vertex data structure. Used during removing degenerate
    // edges(zero-length).
    // Vertex coordinates are: center_x / denom, center_y / denom.
    // Variables: center_x - x-coordinate of the circle's center;
    //            center_y - y-coordinate of the circle's center;
    //            denom - denominator for the previous two values.
    template <typename T>
    class robust_voronoi_vertex {
    public:
        typedef T coordinate_type;
        typedef epsilon_robust_comparator<coordinate_type> erc_type;

        robust_voronoi_vertex(const erc_type &c_x,
                              const erc_type &c_y,
                              const erc_type &den) :
            center_x(c_x),
            center_y(c_y),
            denom(den) {}

        coordinate_type x() const { return center_x.dif() / denom.dif(); }

        coordinate_type y() const { return center_y.dif() / denom.dif(); }

        // Compare two vertices with the given ulp precision.
        bool equals(const robust_voronoi_vertex *that, int ulp) const {
            erc_type lhs1(this->center_x * that->denom);
            erc_type rhs1(this->denom * that->center_x);
            erc_type lhs2(this->center_y * that->denom);
            erc_type rhs2(this->denom * that->center_y);
            return lhs1.compares_undefined(rhs1, ulp) &&
                   lhs2.compares_undefined(rhs2, ulp);
        }

    private:
        erc_type center_x;
        erc_type center_y;
        erc_type denom;
    };

    // Find the x-coordinate (relative to the sweepline position) of the point
    // of the intersection of the horizontal line going through the new site
    // with the arc corresponding to the point site.
    // The relative error is atmost 3EPS.
    template <typename T>
    static double find_distance_to_point_arc(const point_2d<T> &point_site,
                                             const point_2d<T> &new_point) {
        double dx = point_site.x() - new_point.x();
        double dy = point_site.y() - new_point.y();
        return (dx * dx + dy * dy) / (2 * dx);
    }

    // Find the x-coordinate (relative to the sweepline position) of the point
    // of the intersection of the horizontal line going through the new site
    // with the arc corresponding to the segment site.
    // The relative error is atmost 7EPS.
    template <typename T>
    static double find_distance_to_segment_arc(const site_event<T> &segment,
                                               const point_2d<T> &new_point) {
        if (segment.is_vertical()) {
            return (segment.point0().x() - new_point.x()) * 0.5;
        } else {
            const point_2d<T> &segment_start = segment.point1();
            const point_2d<T> &segment_end = segment.point0();
            double a1 = segment_end.x() - segment_start.x();
            double b1 = segment_end.y() - segment_start.y();
            double a3 = new_point.x() - segment_start.x();
            double b3 = new_point.y() - segment_start.y();
            double k = sqrt(a1 * a1 + b1 * b1);
            // Avoid substraction while computing k.
            if (segment.is_inverse()) {
                if (b1 >= 0.0) {
                    k = 1.0 / (b1 + k);
                } else {
                    k = (-b1 + k) / (a1 * a1);
                }
            } else {
                if (b1 >= 0.0) {
                    k = (-b1 - k) / (a1 * a1);
                } else {
                    k = 1.0 / (b1 - k);
                }
            }
            // Relative error of the robust cross product is 1EPS.
            // Relative error of the k is atmost 5EPS.
            // The resulting relative error is atmost 7EPS.
            return robust_cross_product(a1, b1, a3, b3) * k;
        }
    }

    // Robust 65-bit predicate, avoids using high-precision libraries.
    // Works correctly for any input coordinate type that can be casted without
    // lose of data to the integer type within a range [-2^31, 2^31 - 1].
    template <typename T>
    static bool robust_65bit_less_predicate(const point_2d<T> &left_point,
                                            const point_2d<T> &right_point,
                                            const point_2d<T> &new_point) {
        typedef long long ll;
        typedef unsigned long long ull;
        ull a1, a2, b1, b2, b1_sqr, b2_sqr, l_expr, r_expr;
        bool l_expr_plus, r_expr_plus;

        // Compute a1 = (x0-x1), a2 = (x0-x2), b1 = (y0 - y1), b2 = (y0 - y2).
        convert_to_65_bit(new_point.x() - left_point.x(), a1);
        convert_to_65_bit(new_point.x() - right_point.x(), a2);
        convert_to_65_bit(new_point.y() - left_point.y(), b1);
        convert_to_65_bit(new_point.y() - right_point.y(), b2);

        // Compute b1_sqr = (y0-y1)^2 and b2_sqr = (y0-y2)^2.
        b1_sqr = b1 * b1;
        b2_sqr = b2 * b2;
        ull b1_sqr_mod = b1_sqr % a1;
        ull b2_sqr_mod = b2_sqr % a2;

        // Compute l_expr = (x1 - x2).
        l_expr_plus = convert_to_65_bit(left_point.x() - right_point.x(), l_expr);

        // In case (b2_sqr / a2) < (b1_sqr / a1), decrease left_expr by 1.
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
        ull value1 = b1_sqr / a1;
        ull value2 = b2_sqr / a2;
        if (value1 >= value2) {
            r_expr = value1 - value2;
            r_expr_plus = true;
        } else {
            r_expr = value2 - value1;
            r_expr_plus = false;
        }

        // Compare left and right expressions.
        if (!l_expr_plus && r_expr_plus)
            return true;
        if (l_expr_plus && !r_expr_plus)
            return false;
        if (l_expr_plus && r_expr_plus)
            return l_expr < r_expr;
        return l_expr > r_expr;
    }

    // Robust predicate, avoids using high-precision libraries.
    // Returns true if a horizontal line going through the new point site
    // intersects right arc at first, else returns false. If horizontal line
    // goes through intersection point of the given two arcs returns false.
    // Works correctly for any input coordinate type that can be casted without
    // lose of data to the integer type within a range [-2^31, 2^31-1].
    template <typename T>
    static bool less_predicate(const point_2d<T> &left_point,
                               const point_2d<T> &right_point,
                               const point_2d<T> &new_point) {
        // Any two point sites with different x-coordinates create two
        // bisectors. Each bisector is defined by the order the sites
        // appear in its representation. Predicates used in this function
        // won't produce the correct result for the any arrangment of the
        // input sites. That's why some preprocessing is required to handle
        // such cases.
        if (left_point.x() > right_point.x()) {
            if (new_point.y() <= left_point.y())
                return false;
        } else if (left_point.x() < right_point.x()) {
            if (new_point.y() >= right_point.y())
                return true;
        } else {
            // If x-coordinates of the sites are equal, we may produce the
            // result without any further computations.
            return left_point.y() + right_point.y() < 2.0 * new_point.y();
        }

        double dist1 = find_distance_to_point_arc(left_point, new_point);
        double dist2 = find_distance_to_point_arc(right_point, new_point);

        if (!almost_equal(dist1, dist2, 6))
            return dist1 < dist2;

        // If the result of the epsilon robust predicate is undefined
        // use 65-bit robust predicate that produces exact result.
        return robust_65bit_less_predicate(left_point, right_point, new_point);
    }

    template <typename T>
    static kPredicateResult fast_less_predicate(point_2d<T> site_point, site_event<T> segment_site,
                                                point_2d<T> new_point, bool reverse_order) {
        typedef long long ll;
        typedef unsigned long long ull;
        if (orientation_test(segment_site.point0(true), segment_site.point1(true),
            new_point) != RIGHT_ORIENTATION) {
            return (!segment_site.is_inverse()) ? LESS : MORE;
        }

        const point_2d<T> &segment_start = segment_site.point0(true);
        const point_2d<T> &segment_end = segment_site.point1(true);
        double dif_x = new_point.x() - site_point.x();
        double dif_y = new_point.y() - site_point.y();
        double a = segment_end.x() - segment_start.x();
        double b = segment_end.y() - segment_start.y();

        if (segment_site.is_vertical()) {
            if (new_point.y() < site_point.y() && !reverse_order)
                return MORE;
            else if (new_point.y() > site_point.y() && reverse_order)
                return LESS;
            return UNDEFINED;
        } else {
            kOrientation orientation = orientation_test(a, b, dif_x, dif_y);
            if (orientation == LEFT_ORIENTATION) {
                if (!segment_site.is_inverse())
                    return reverse_order ? LESS : UNDEFINED;
                return reverse_order ? UNDEFINED : MORE;
            }
        }

        double fast_left_expr = a * (dif_y + dif_x) * (dif_y - dif_x);
        double fast_right_expr = (2.0 * b) * dif_x * dif_y;
        if (!(almost_equal(fast_left_expr, fast_right_expr, 4))) {
            if ((fast_left_expr > fast_right_expr) ^ reverse_order)
                return reverse_order ? LESS : MORE;
            return UNDEFINED;
        }
        return UNDEFINED;
    }

//#ifdef USE_MULTI_PRECISION_LIBRARY
//    template<typename T>
//    static bool mpz_less_predicate(point_2d<T> segment_start, point_2d<T> segment_end,
//                                   point_2d<T> site_point, point_2d<T> new_point,
//                                   bool reverse_order) {
//        mpz_class segment_start_x, segment_start_y, segment_end_x, segment_end_y,
//                  site_point_x, site_point_y, new_point_x, new_point_y;
//        segment_start_x = static_cast<int>(segment_start.x());
//        segment_start_y = static_cast<int>(segment_start.y());
//        segment_end_x = static_cast<int>(segment_end.x());
//        segment_end_y = static_cast<int>(segment_end.y());
//        site_point_x = static_cast<int>(site_point.x());
//        site_point_y = static_cast<int>(site_point.y());
//        new_point_x = static_cast<int>(new_point.x());
//        new_point_y = static_cast<int>(new_point.y());
//
//        mpz_class dif_x, dif_y, a, b, mul1, mul2, temp, left_expr, right_expr;
//        dif_x = new_point_x - site_point_x;
//        dif_y = new_point_y - site_point_y;
//        a = segment_end_x - segment_start_x;
//        b = segment_end_y - segment_start_y;
//        mul1 = new_point_x - segment_end_x;
//        mul2 = new_point_y - segment_end_y;
//        temp = dif_x * dif_x + dif_y * dif_y;
//        left_expr = (a * a + b * b) * temp * temp;
//        right_expr = (2.0 * dif_x * (b * mul1 - a * mul2) - b * temp);
//        right_expr = right_expr * right_expr;
//
//        return (!reverse_order) ? (left_expr > right_expr) : (left_expr < right_expr);
//    }
//#endif

    // Returns true if a horizontal line going through a new site intersects
    // right arc at first, else returns false. If horizontal line goes
    // through intersection point of the given two arcs returns false also.
    // reverse_order flag defines arrangement of the sites. If it's false
    // the order is (point, segment), else - (segment, point).
    template <typename T>
    static bool less_predicate(point_2d<T> site_point, site_event<T> segment_site,
                               point_2d<T> new_point, bool reverse_order) {
        kPredicateResult fast_res = fast_less_predicate(
            site_point, segment_site, new_point, reverse_order);
        if (fast_res != UNDEFINED) {
            return (fast_res == LESS);
        }

        double dist1 = find_distance_to_point_arc(site_point, new_point);
        double dist2 = find_distance_to_segment_arc(segment_site, new_point);

        if (!almost_equal(dist1, dist2, 10)) {
            return reverse_order ^ (dist1 < dist2);
        }

        return reverse_order ^ (dist1 < dist2);
    }

    // Returns true if a horizontal line going through a new site intersects
    // right arc at first, else returns false. If horizontal line goes
    // through intersection point of the given two arcs returns false also.
    template <typename T>
    static bool less_predicate(site_event<T> left_segment,
                               site_event<T> right_segment,
                               point_2d<T> new_point) {
        // Handle temporary segment sites.
        if (left_segment.index() == right_segment.index()) {
            return orientation_test(left_segment.point0(),
                                    left_segment.point1(),
                                    new_point) == LEFT_ORIENTATION;
        }

        // Distances between the x-coordinate of the sweepline and
        // the x-coordinates of the points of the intersections of the
        // horizontal line going through the new site with arcs corresponding
        // to the first and to the second segment sites respectively.
        double dist1 = find_distance_to_segment_arc(left_segment, new_point);
        double dist2 = find_distance_to_segment_arc(right_segment, new_point);

        // The undefined ulp range is equal to 7EPS + 7EPS <= 14ULP.
        if (!almost_equal(dist1, dist2, 14)) {
            return dist1 < dist2;
        }

        // TODO(asydorchuk): Add mpl support there.
        return dist1 < dist2;
    }

    ///////////////////////////////////////////////////////////////////////////
    // CIRCLE EVENTS CREATION /////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // At the moment all the circle event creation functions use the
    // epsilon_robust_comparator class to output the parameters of the
    // inscribed circle. Such approach may produce incorrect results while
    // comparing two circle events. In such cases high-precision functions
    // are required to recalculate circle's parameters.

    template <typename T>
    static inline T sqr_distance(T dif_x, T dif_y) {
        return dif_x * dif_x + dif_y * dif_y;
    }

    // Find parameters of the inscribed circle that is tangent to three
    // point sites.
    template <typename T>
    static bool create_circle_event_ppp(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        circle_event<T> &c_event) {
        double dif_x1 = site1.x() - site2.x();
        double dif_x2 = site2.x() - site3.x();
        double dif_y1 = site1.y() - site2.y();
        double dif_y2 = site2.y() - site3.y();
        double orientation = robust_cross_product(dif_x1, dif_y1, dif_x2, dif_y2);
        if (orientation_test(orientation) != RIGHT_ORIENTATION)
            return false;
        double inv_orientation = 0.5 / orientation;
        double sum_x1 = site1.x() + site2.x();
        double sum_x2 = site2.x() + site3.x();
        double sum_y1 = site1.y() + site2.y();
        double sum_y2 = site2.y() + site3.y();
        double dif_x3 = site1.x() - site3.x();
        double dif_y3 = site1.y() - site3.y();
        epsilon_robust_comparator<T> c_x, c_y;
        c_x += dif_x1 * sum_x1 * dif_y2;
        c_x += dif_y1 * sum_y1 * dif_y2;
        c_x -= dif_x2 * sum_x2 * dif_y1;
        c_x -= dif_y2 * sum_y2 * dif_y1;
        c_y += dif_x2 * sum_x2 * dif_x1;
        c_y += dif_y2 * sum_y2 * dif_x1;
        c_y -= dif_x1 * sum_x1 * dif_x2;
        c_y -= dif_y1 * sum_y1 * dif_x2;
        c_x *= inv_orientation;
        c_y *= inv_orientation;
        epsilon_robust_comparator<T> lower_x(c_x);
        lower_x += sqrt(sqr_distance(dif_x1, dif_y1) * sqr_distance(dif_x2, dif_y2) *
                        sqr_distance(dif_x3, dif_y3)) * fabs(inv_orientation);
        c_event = circle_event<double>(c_x, c_y, lower_x);
        return true;
    }

    // Find parameters of the inscribed circle that is tangent to two
    // point sites and on segment site.
    template <typename T>
    static bool create_circle_event_pps(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        int segment_index,
                                        circle_event<T> &c_event) {
        if (segment_index != 2) {
            kOrientation orient1 = orientation_test(site1.point0(),
                site2.point0(), site3.point0(true));
            kOrientation orient2 = orientation_test(site1.point0(),
                site2.point0(), site3.point1(true));
            if (segment_index == 1 && site1.x0() >= site2.x0()) {
                if (orient1 != RIGHT_ORIENTATION)
                    return false;
            } else if (segment_index == 3 && site2.x0() >= site1.x0()) {
                if (orient2 != RIGHT_ORIENTATION)
                    return false;
            } else if (orient1 != RIGHT_ORIENTATION && orient2 != RIGHT_ORIENTATION) {
                return false;
            }
        } else {
            if (site3.point0(true) == site1.point0() &&
                site3.point1(true) == site2.point0())
                return false;
        }

        double line_a = site3.point1().y() - site3.point0().y();
        double line_b = site3.point0().x() - site3.point1().x();
        double vec_x = site2.y() - site1.y();
        double vec_y = site1.x() - site2.x();
        double teta = robust_cross_product(line_a, line_b, -vec_y, vec_x);
        double A = robust_cross_product(line_a, line_b,
            site3.point1().y() - site1.y(),
            site1.x() - site3.point1().x());
        double B = robust_cross_product(line_a, line_b,
            site3.point1().y() - site2.y(),
            site2.x() - site3.point1().x());
        double denom = robust_cross_product(vec_x, vec_y, line_a, line_b);
        double inv_segm_len = 1.0 / sqrt(sqr_distance(line_a, line_b));
        epsilon_robust_comparator<double> t;
        if (orientation_test(denom) == COLLINEAR) {
            t += teta / (4.0 * (A + B));
            t -= A * B / (teta * (A + B));
        } else {
            double det = sqrt((teta * teta + denom * denom) * A * B);
            if (segment_index == 2) {
                t -= det / (denom * denom);
            } else {
                t += det / (denom * denom);
            }
            t += teta * (A + B) / (2.0 * denom * denom);
        }
        epsilon_robust_comparator<double> c_x, c_y;
        c_x += 0.5 * (site1.x() + site2.x());
        c_x += t * vec_x;
        c_y += 0.5 * (site1.y() + site2.y());
        c_y += t * vec_y;
        epsilon_robust_comparator<double> r, lower_x(c_x);
        r -= line_a * site3.x0();
        r -= line_b * site3.y0();
        r += line_a * c_x;
        r += line_b * c_y;
        r.abs();
        lower_x += r * inv_segm_len;
        c_event = circle_event<double>(c_x, c_y, lower_x);
        return true;
    }

    // Find parameters of the inscribed circle that is tangent to one
    // point site and two segment sites.
    template <typename T>
    static bool create_circle_event_pss(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        int point_index,
                                        circle_event<T> &c_event) {
        if (site2.index() == site3.index()) {
            return false;
        }

        const point_2d<T> &segm_start1 = site2.point1(true);
        const point_2d<T> &segm_end1 = site2.point0(true);
        const point_2d<T> &segm_start2 = site3.point0(true);
        const point_2d<T> &segm_end2 = site3.point1(true);

        if (point_index == 2) {
            if (!site2.is_inverse() && site3.is_inverse())
                return false;
            if (site2.is_inverse() == site3.is_inverse() &&
                orientation_test(segm_end1, site1.point0(), segm_end2) != RIGHT_ORIENTATION)
                return false;
        }

        double a1 = segm_end1.x() - segm_start1.x();
        double b1 = segm_end1.y() - segm_start1.y();
        double a2 = segm_end2.x() - segm_start2.x();
        double b2 = segm_end2.y() - segm_start2.y();
        double orientation = robust_cross_product(b1, a1, b2, a2);
        if (orientation_test(orientation) == COLLINEAR) {
            double a = a1 * a1 + b1 * b1;
            double c = robust_cross_product(b1, a1, segm_start2.y() - segm_start1.y(),
                                            segm_start2.x() - segm_start1.x());
            double det = robust_cross_product(a1, b1, site1.x() - segm_start1.x(),
                                              site1.y() - segm_start1.y()) *
                         robust_cross_product(b1, a1, site1.y() - segm_start2.y(),
                                              site1.x() - segm_start2.x());
            epsilon_robust_comparator<double> t;
            t -= a1 * ((segm_start1.x() + segm_start2.x()) * 0.5 - site1.x());
            t -= b1 * ((segm_start1.y() + segm_start2.y()) * 0.5 - site1.y());
            if (point_index == 2) {
                t += sqrt(det);
            } else {
                t -= sqrt(det);
            }
            t /= a;
            epsilon_robust_comparator<double> c_x, c_y;
            c_x += 0.5 * (segm_start1.x() + segm_start2.x());
            c_x += a1 * t;
            c_y += 0.5 * (segm_start1.y() + segm_start2.y());
            c_y += b1 * t;
            epsilon_robust_comparator<double> lower_x(c_x);
            lower_x += 0.5 * fabs(c) / sqrt(a);
            c_event = circle_event<double>(c_x, c_y, lower_x);
            return true;
        } else {
            double sqr_sum1 = sqrt(a1 * a1 + b1 * b1);
            double sqr_sum2 = sqrt(a2 * a2 + b2 * b2);
            double a = robust_cross_product(a1, b1, -b2, a2);
            if (a >= 0) {
                a += sqr_sum1 * sqr_sum2;
            } else {
                a = (orientation * orientation) / (sqr_sum1 * sqr_sum2 - a);
            }
            double or1 = robust_cross_product(b1, a1, segm_end1.y() - site1.y(),
                                              segm_end1.x() - site1.x());
            double or2 = robust_cross_product(a2, b2, segm_end2.x() - site1.x(),
                                              segm_end2.y() - site1.y());
            double det = 2.0 * a * or1 * or2;
            double c1 = robust_cross_product(b1, a1, segm_end1.y(), segm_end1.x());
            double c2 = robust_cross_product(a2, b2, segm_end2.x(), segm_end2.y());
            double inv_orientation = 1.0 / orientation;
            epsilon_robust_comparator<double> t, b, ix, iy;
            ix += c1 * a2 * inv_orientation;
            ix += a1 * c2 * inv_orientation;
            iy += b1 * c2 * inv_orientation;
            iy += b2 * c1 * inv_orientation;
            b += ix * (a1 * sqr_sum2);
            b += ix * (a2 * sqr_sum1);
            b += iy * (b1 * sqr_sum2);
            b += iy * (b2 * sqr_sum1);
            b -= sqr_sum1 * robust_cross_product(a2, b2, -site1.y(), site1.x());
            b -= sqr_sum2 * robust_cross_product(a1, b1, -site1.y(), site1.x());
            t -= b;
            if (point_index == 2) {
                t += sqrt(det);
            } else {
                t -= sqrt(det);
            }
            t /= (a * a);
            epsilon_robust_comparator<double> c_x(ix), c_y(iy);
            c_x += t * (a1 * sqr_sum2);
            c_x += t * (a2 * sqr_sum1);
            c_y += t * (b1 * sqr_sum2);
            c_y += t * (b2 * sqr_sum1);
            t.abs();
            epsilon_robust_comparator<double> lower_x(c_x);
            lower_x += t * fabs(orientation);
            c_event = circle_event<double>(c_x, c_y, lower_x);
        }
        return true;
    }

    // Find parameters of the inscribed circle that is tangent to three
    // segment sites.
    template <typename T>
    static bool create_circle_event_sss(const site_event<T> &site1,
                                        const site_event<T> &site2,
                                        const site_event<T> &site3,
                                        circle_event<T> &c_event) {
        if (site1.index() == site2.index() || site2.index() == site3.index()) {
            return false;
        }
        double a1 = site1.x1(true) - site1.x0(true);
        double b1 = site1.y1(true) - site1.y0(true);
        double c1 = robust_cross_product(b1, a1, site1.y0(true), site1.x0(true));
        double a2 = site2.x1(true) - site2.x0(true);
        double b2 = site2.y1(true) - site2.y0(true);
        double c2 = robust_cross_product(b2, a2, site2.y0(true), site2.x0(true));
        double a3 = site3.x1(true) - site3.x0(true);
        double b3 = site3.y1(true) - site3.y0(true);
        double c3 = robust_cross_product(b3, a3, site3.y0(true), site3.x0(true));
        double len1 = sqrt(a1 * a1 + b1 * b1);
        double len2 = sqrt(a2 * a2 + b2 * b2);
        double len3 = sqrt(a3 * a3 + b3 * b3);
        double cross_12 = robust_cross_product(a1, b1, a2, b2);
        double cross_23 = robust_cross_product(a2, b2, a3, b3);
        double cross_31 = robust_cross_product(a3, b3, a1, b1);
        epsilon_robust_comparator<double> det, c_x, c_y, r;
        det += cross_12 * len3;
        det += cross_23 * len1;
        det += cross_31 * len2;
        c_x += a1 * c2 * len3;
        c_x -= a2 * c1 * len3;
        c_x += a2 * c3 * len1;
        c_x -= a3 * c2 * len1;
        c_x += a3 * c1 * len2;
        c_x -= a1 * c3 * len2;
        c_y += b1 * c2 * len3;
        c_y -= b2 * c1 * len3;
        c_y += b2 * c3 * len1;
        c_y -= b3 * c2 * len1;
        c_y += b3 * c1 * len2;
        c_y -= b1 * c3 * len2;
        r += b2 * c_x;
        r -= a2 * c_y;
        r -= c2 * det;
        r /= len2;
        c_event = circle_event<double>(c_x, c_y, c_x + r, det);
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI BEACH LINE DATA TYPES //////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // Represents a bisector node made by two arcs that correspond to the left
    // and right sites. Arc is defined as a curve with points equidistant from
    // the site and from the sweepline. If the site is a point then the arc is
    // a parabola, otherwise it's a line segment. A segment site event will
    // produce different bisectors depending on its direction.
    // In the general case two sites will create two opposite bisectors. That's
    // why the order of the sites is important to define the unique bisector.
    // The one site is considered to be newer than the other in case it was
    // processed by the algorithm later.
    template <typename T>
    class beach_line_node {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> point_2d_type;
        typedef site_event<T> site_event_type;

        beach_line_node() {}

        // Constructs degenerate bisector, used to search an arc that is above
        // the given site. The input to the constructor is the new site point.
        explicit beach_line_node(const site_event_type &new_point) {
            left_site_ = new_point;
            right_site_ = new_point;
        }

        // Constructs a new bisector. The input to the constructor is the two
        // sites that create the bisector. The order of sites is important.
        beach_line_node(const site_event_type &left_point,
                        const site_event_type &right_point) {
            left_site_ = left_point;
            right_site_ = right_point;
        }

        const site_event_type &left_site() const {
            return left_site_;
        }

        const site_event_type &right_site() const {
            return right_site_;
        }

        void left_site(const site_event_type &site) {
            left_site_ = site;
        }

        void right_site(const site_event_type &site) {
            right_site_ = site;
        }

        void inverse_left_site() {
            left_site_.inverse();
        }

        void inverse_right_site() {
            right_site_.inverse();
        }

        // Get the x coordinate of the newer site.
        coordinate_type comparison_x() const {
            return (left_site_.index() >= right_site_.index()) ?
                   left_site_.x() : right_site_.x();
        }

        // Get comparison pair: y coordinate and direction of the newer site.
        std::pair<coordinate_type, int> comparison_y(bool is_new_node = true) const {
            if (left_site_.index() == right_site_.index()) {
                return std::make_pair(left_site_.y(), 0);
            }
            if (left_site_.index() > right_site_.index()) {
                if (!is_new_node && left_site_.is_segment() && left_site_.is_vertical()) {
                    return std::make_pair(left_site_.y1(), 1);
                }
                return std::make_pair(left_site_.y(), 1);
            }
            return std::make_pair(right_site_.y(), -1);
        }

        // Get the index of the newer site.
        int comparison_index() const {
            return (left_site_.index() > right_site_.index()) ?
                    left_site_.index() : right_site_.index();
        }

        // Get the newer site.
        const site_event_type &comparison_site() const {
            return (left_site_.index() >= right_site_.index()) ?
                   left_site_ : right_site_;
        }

        // Return true if the horizontal line going through the new site
        // intersects the arc corresponding to the right_site before the
        // arc corresponding to the left_site.
        bool less(const point_2d_type &new_site) const {
            if (!left_site_.is_segment()) {
                return (!right_site_.is_segment()) ? less_pp(new_site) : less_ps(new_site);
            } else {
                return (!right_site_.is_segment()) ? less_sp(new_site) : less_ss(new_site);
            }
        }

    private:
        bool less_pp(const point_2d_type &new_site) const {
            return less_predicate(left_site_.point0(), right_site_.point0(), new_site);
        }

        bool less_ps(const point_2d_type &new_site) const {
            return less_predicate(left_site_.point0(), right_site_, new_site, false);
        }

        bool less_sp(const point_2d_type &new_site) const {
            return less_predicate(right_site_.point0(), left_site_, new_site, true);
        }

        bool less_ss(const point_2d_type &new_site) const {
            return less_predicate(left_site_, right_site_, new_site);
        }

        site_event_type left_site_;
        site_event_type right_site_;
    };

    // Represents edge data sturcture from the voronoi output, that is
    // associated as a value with beach line bisector as a key in the beach
    // line. Contains iterator to the circle event in the circle event
    // queue in case it's the second bisector that forms a circle event.
    template <typename T>
    class beach_line_node_data {
    public:
        explicit beach_line_node_data(voronoi_edge<T> *new_edge) :
            edge_(new_edge),
            contains_circle_event_(false) {}

        void activate_circle_event(typename circle_events_queue<T>::circle_event_type_it it) {
            circle_event_it_ = it;
            contains_circle_event_ = true;
        }

        void deactivate_circle_event() {
            if (contains_circle_event_)
                circle_event_it_->deactivate();
            contains_circle_event_ = false;
        }

        voronoi_edge<T> *edge() const {
            return edge_;
        }

        void edge(voronoi_edge<T> *new_edge) {
            edge_ = new_edge;
        }
    private:
        typename circle_events_queue<T>::circle_event_type_it circle_event_it_;
        voronoi_edge<T> *edge_;
        bool contains_circle_event_;
    };

    // Beach line comparison functor.
    template <typename BeachLineNode>
    struct node_comparer {
    public:
        typedef typename BeachLineNode::coordinate_type coordinate_type;

        // Compares nodes in the balanced binary search tree. Nodes are
        // compared based on the y coordinates of the arcs intersection points.
        // Nodes with less y coordinate of the intersection point go first.
        // Comparison is only called during the new site events processing.
        // That's why one of the nodes will always lie on the sweepline and may
        // be represented as a straight horizontal line.
        bool operator() (const BeachLineNode &node1,
                         const BeachLineNode &node2) const {
            // Get x coordinate of the righmost site from both nodes.
            coordinate_type node1_x = node1.comparison_x();
            coordinate_type node2_x = node2.comparison_x();

            if (node1_x < node2_x) {
                // The second node contains a new site.
                return node1.less(node2.comparison_site().point0());
            } else if (node1_x > node2_x) {
                // The first node contains a new site.
                return !node2.less(node1.comparison_site().point0());
            } else {
                // This checks were evaluated experimentally.
                if (node1.comparison_index() == node2.comparison_index()) {
                    // Both nodes are new (inserted during the same site event
                    // processing).
                    return node1.comparison_y() < node2.comparison_y();
                } else if (node1.comparison_index() < node2.comparison_index()) {
                    std::pair<coordinate_type, int> y1 = node1.comparison_y(false);
                    std::pair<coordinate_type, int> y2 = node2.comparison_y();
                    if (y1.first != y2.first) {
                        return y1.first < y2.first;
                    }
                    return (!node1.comparison_site().is_segment()) ? (y1.second < 0) : false;
                } else {
                    std::pair<coordinate_type, int> y1 = node1.comparison_y();
                    std::pair<coordinate_type, int> y2 = node2.comparison_y(false);
                    if (y1.first != y2.first) {
                        return y1.first < y2.first;
                    }
                    return (!node2.comparison_site().is_segment()) ? (y2.second > 0) : true;
                }
            }
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI BUILDER ////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // The sweepline algorithm implementation to compute voronoi diagram of
    // input data sets of points and segments (Fortune's algorithm).
    // Complexity - O(N*logN), memory usage - O(N), where N is the total
    // number of input objects.
    // Sweepline is a vertical line that sweeps from the left to the right
    // along the x-axis positive direction. Each of the input objects is
    // wrapped into the site event. Each event is characterized by its
    // coordinates: the point site is defined by the point itself,
    // the segment site is defined by its startpoint. At any moment we
    // consider only the sites that lie to the left of the sweepline. Beach
    // line is a curve formed by the parabolic arcs and line segments, that
    // consists of the points equidistant from the sweepline and the nearest
    // site to the left of the sweepline. The part of the generated diagram to
    // the left of the beach line remains unchanged until the end of the
    // algorithm. Each node in the beach line corresponds to some voronoi edge.
    // Each node is represented by two sites. Two neighboring nodes has a
    // common site. Circle event appears at the rightmost point of the circle
    // tangent to the three sites that correspond to the two consecutive
    // bisectors. At each step algorithm goes over the leftmost event
    // and processes it appropriately. This is made until there are no events.
    // At the end output data structure holds the voronoi diagram of the
    // initial set of objects.
    // Each point creates one site event. Each segment creates three site
    // events: two for its endpoints and one for the segment itself (this is
    // made to simplify output construction). All the site events are
    // constructed at the algorithm initialization step. After that they
    // are ordered using quicksort algorithm.
    // Priority queue is used to dynamically hold circle events. At each step
    // of the algorithm the leftmost event is retrieved by comparing the
    // current site event and the topmost element from the circle event queue.
    // Standard map container was chosen to hold state of the beach line. The
    // keys of the map correspond to the bisectors and values to the
    // corresponding edges from the output data structure. Specially defined
    // comparison functor is used to make the beach line correctly ordered.
    // Epsilon-based and high-precision approaches are used to guarantee
    // efficiency and robustness of the algorithm implementation.
    // Member data: 1) site_events_ - vector of the site events;
    //              2) site_event_iterator_ - iterator to the next
    //                 site event;
    //              3) circle_events_ - priority queue of circle events,
    //                 allows to retrieve topmost event in O(logN) time;
    //              4) beach_line_ - contains current state of the beach line;
    //              5) end_points_ - maps endpoints of the segment sites with
    //                 temporary nodes from the beach line. While sweepline
    //                 sweeps over the second endpoint of the segments
    //                 temporary nodes are being removed;
    //              6) output_ - contains voronoi diagram itself.
    template <typename T>
    class voronoi_builder {
    public:
        typedef T coordinate_type;
        typedef point_2d<coordinate_type> point_2d_type;
        typedef voronoi_output<coordinate_type> output_type;
        typedef site_event<coordinate_type> site_event_type;
        typedef circle_event<coordinate_type> circle_event_type;

        voronoi_builder(output_type &output) : output_(output) {
            // Avoid algorithm fails in case we forgot to init the builder.
            site_event_iterator_ = site_events_.begin();
        }

        // Create site events.
        // There will be one site event for each input point and three site
        // events for each input segment (both endpoints of a segment and the
        // segment itself).
        template <typename iType>
        void init(const std::vector< point_2d<iType> > &points,
                  const std::vector< std::pair< point_2d<iType>, point_2d<iType> > > &segments) {
            typedef std::pair< point_2d<iType>, point_2d<iType> > iSegment2D;

            // Clear output.
            output_.clear();

            // TODO(asydorchuk): We may add segment intersection checks there.

            // Avoid additional memory reallocations.
            site_events_.reserve(points.size() + segments.size() * 3);

            // Create a site event from each input point.
            for (typename std::vector< point_2d<iType> >::const_iterator it = points.begin();
                 it != points.end(); it++) {
                site_events_.push_back(make_site_event(static_cast<T>(it->x()),
                                                       static_cast<T>(it->y()),
                                                       0));
            }

            // Each segment creates three segment sites:
            //   1) the startpoint of the segment;
            //   2) the endpoint of the segment;
            //   3) the segment itself.
            for (typename std::vector<iSegment2D>::const_iterator it = segments.begin();
                 it != segments.end(); it++) {
                T x1 = static_cast<T>(it->first.x());
                T y1 = static_cast<T>(it->first.y());
                T x2 = static_cast<T>(it->second.x());
                T y2 = static_cast<T>(it->second.y());
                site_events_.push_back(make_site_event(x1, y1, 0));
                site_events_.push_back(make_site_event(x2, y2, 0));
                site_events_.push_back(make_site_event(x1, y1, x2, y2, 0));
            }

            // Sort the site events.
            sort(site_events_.begin(), site_events_.end());

            // Remove duplicates.
            site_events_.erase(unique(
                site_events_.begin(), site_events_.end()), site_events_.end());

            // Number the sites.
            for (size_t cur = 0; cur < site_events_.size(); cur++)
                site_events_[cur].index(cur);

            // Init the site's iterator.
            site_event_iterator_ = site_events_.begin();

            // Init the output data structure.
            output_.init(site_events_.size());
        }

        void clear() {
            beach_line_.clear();
            circle_events_.clear();
            site_events_.clear();
        }

        // Run the sweepline algorithm.
        void run_sweepline() {
            // Init the beach line.
            if (site_events_.empty()) {
                // No input sites.
                return;
            } else if (site_events_.size() == 1) {
                // Handle one input site case.
                output_.process_single_site(site_events_[0]);
                site_event_iterator_++;
            } else {
                int skip = 0;

                // Count the number of the sites to init the beach line.
                while(site_event_iterator_ != site_events_.end() &&
                      site_event_iterator_->x() == site_events_.begin()->x() &&
                      site_event_iterator_->is_vertical()) {
                    site_event_iterator_++;
                    skip++;
                }

                if (skip == 1) {
                    // Init the beach line with the two first sites.
                    init_beach_line();
                } else {
                    // Init the beach line with the sites situated on the same
                    // vertical line. This could be a set of point and vertical
                    // segment sites.
                    init_beach_line_collinear_sites();
                }
            }

            // The algorithm stops when there are no events to process.
            // The circle events with the same rightmost point as the next
            // site event go first.
            while (!circle_events_.empty() ||
                   !(site_event_iterator_ == site_events_.end())) {
                if (circle_events_.empty()) {
                    process_site_event();
                } else if (site_event_iterator_ == site_events_.end()) {
                    process_circle_event();
                } else {
                    if (circle_events_.top().compare(*site_event_iterator_) == MORE) {
                        process_site_event();
                    } else {
                        process_circle_event();
                    }
                }
            }

            // Simplify the output (remove zero-length edges).
            output_.simplify();
            clear();
        }

    private:
        typedef typename std::vector<site_event_type>::const_iterator
            site_event_iterator_type;
        typedef typename output_type::voronoi_edge_type edge_type;
        typedef circle_events_queue<coordinate_type> circle_event_queue_type;
        typedef beach_line_node<coordinate_type> key_type;
        typedef beach_line_node_data<coordinate_type> value_type;
        typedef node_comparer<key_type> node_comparer_type;
        typedef std::map< key_type, value_type, node_comparer_type >
            beach_line_type;
        typedef typename beach_line_type::iterator beach_line_iterator;
        typedef std::pair<point_2d_type, beach_line_iterator> end_point_type;

        // Init the beach line with the two first sites.
        // The first site is always a point.
        void init_beach_line() {
            // Get the first and the second site events.
            site_event_iterator_type it_first = site_events_.begin();
            site_event_iterator_type it_second = site_events_.begin();
            it_second++;

            // Update the beach line.
            insert_new_arc(*it_first, *it_first, *it_second, beach_line_.begin());

            // The second site has been already processed.
            // Move the site's iterator.
            site_event_iterator_++;
        }

        // Insert bisectors for all collinear initial sites.
        void init_beach_line_collinear_sites() {
             site_event_iterator_type it_first = site_events_.begin();
             site_event_iterator_type it_second = site_events_.begin();
             it_second++;
             while (it_second != site_event_iterator_) {
                 // Create a new beach line node.
                 key_type new_node(*it_first, *it_second);

                 // Update the output.
                 edge_type *edge = output_.insert_new_edge(*it_first, *it_second);

                 // Insert a new bisector into the beach line.
                 beach_line_.insert(
                     std::pair<key_type, value_type>(new_node, value_type(edge)));

                 // Update iterators.
                 it_first++;
                 it_second++;
             }
        }

        // Process a single site.
        void process_site_event() {
            // Get the site event to process.
            site_event_type site_event = *site_event_iterator_;

            // Move the site's iterator.
            site_event_iterator_++;

            // If a new site is an end point of some segment,
            // remove temporary nodes from the beach line data structure.
            if (!site_event.is_segment()) {
                while (!end_points_.empty() &&
                       end_points_.top().first == site_event.point0()) {
                    beach_line_.erase(end_points_.top().second);
                    end_points_.pop();
                }
            }

            // Create degenerate node.
            key_type new_key(site_event);

            // Find the node in the binary search tree with left arc
            // lying above the new site point.
            beach_line_iterator it = beach_line_.lower_bound(new_key);
            int it_dist = site_event.is_segment() ? 2 : 1;

            // Do further processing depending on the above node position.
            // For any two neighbouring nodes the second site of the first node
            // is the same as the first site of the second arc.
            if (it == beach_line_.end()) {
                // The above arc corresponds to the second arc of the last node.
                // Move the iterator to the last node.
                it--;

                // Get the second site of the last node
                const site_event_type &site_arc = it->first.right_site();

                // Insert new nodes into the beach line. Update the output.
                beach_line_iterator new_node_it =
                    insert_new_arc(site_arc, site_arc, site_event, it);

                // Add a candidate circle to the circle event queue.
                // There could be only one new circle event formed by
                // a new bisector and the one on the left.
                std::advance(new_node_it, -it_dist);
                activate_circle_event(it->first.left_site(),
                                      it->first.right_site(),
                                      site_event, new_node_it);
            } else if (it == beach_line_.begin()) {
                // The above arc corresponds to the first site of the first node.
                const site_event_type &site_arc = it->first.left_site();

                // Insert new nodes into the beach line. Update the output.
                insert_new_arc(site_arc, site_arc, site_event, it);

                // If the site event is a segment, update its direction.
                if (site_event.is_segment()) {
                    site_event.inverse();
                }

                // Add a candidate circle to the circle event queue.
                // There could be only one new circle event formed by
                // a new bisector and the one on the right.
                activate_circle_event(site_event, it->first.left_site(),
                                      it->first.right_site(), it);
            } else {
                // The above arc corresponds neither to the first,
                // nor to the last site in the beach line.
                const site_event_type &site_arc2 = it->first.left_site();
                const site_event_type &site3 = it->first.right_site();

                // Remove the candidate circle from the event queue.
                it->second.deactivate_circle_event();
                it--;
                const site_event_type &site_arc1 = it->first.right_site();
                const site_event_type &site1 = it->first.left_site();

                // Insert new nodes into the beach line. Update the output.
                beach_line_iterator new_node_it =
                    insert_new_arc(site_arc1, site_arc2, site_event, it);

                // Add candidate circles to the circle event queue.
                // There could be up to two circle events formed by
                // a new bisector and the one on the left or right.
                std::advance(new_node_it, -it_dist);
                activate_circle_event(site1, site_arc1, site_event,
                                      new_node_it);

                // If the site event is a segment, update its direction.
                if (site_event.is_segment()) {
                    site_event.inverse();
                }
                std::advance(new_node_it, it_dist + 1);
                activate_circle_event(site_event, site_arc2, site3,
                                      new_node_it);
            }
        }

        // Process a single circle event.
        // In general case circle event is made of the three consequtive sites
        // that form two bisector nodes in the beach line data structure.
        // Let circle event sites be A, B, C, two bisectors that define
        // circle event be (A, B), (B, C). During circle event processing
        // we remove (A, B), (B, C) and insert (A, C). As beach line comparer
        // works correctly only if one of the nodes is a new one we remove
        // (B, C) bisector and change (A, B) bisector to the (A, C). That's
        // why we use const_cast there and take all the responsibility that
        // map data structure keeps correct ordering.
        void process_circle_event() {
            // Get the topmost circle event.
            const circle_event_type &circle_event = circle_events_.top();
            beach_line_iterator it_first = circle_event.bisector();
            beach_line_iterator it_last = it_first;

            // Get the C site.
            site_event_type site3 = it_first->first.right_site();

            // Get the half-edge corresponding to the second bisector - (B, C).
            edge_type *bisector2 = it_first->second.edge();

            // Get the half-edge corresponding to the first bisector - (A, B).
            it_first--;
            edge_type *bisector1 = it_first->second.edge();

            // Get the A site.
            site_event_type site1 = it_first->first.left_site();

            if (!site1.is_segment() && site3.is_segment() &&
                site3.point1(true) == site1.point0()) {
                site3.inverse();
            }

            // Change the (A, B) bisector node to the (A, C) bisector node.
            const_cast<key_type &>(it_first->first).right_site(site3);

            // Insert the new bisector into the beach line.
            it_first->second.edge(output_.insert_new_edge(site1, site3, circle_event,
                                                          bisector1, bisector2));

            // Remove the (B, C) bisector node from the beach line.
            beach_line_.erase(it_last);
            it_last = it_first;

            // Pop the topmost circle event from the event queue.
            circle_events_.pop();

            // Check new triplets formed by the neighboring arcs
            // to the left for potential circle events.
            if (it_first != beach_line_.begin()) {
                it_first->second.deactivate_circle_event();
                it_first--;
                const site_event_type &site_l1 = it_first->first.left_site();
                activate_circle_event(site_l1, site1, site3, it_last);
            }

            // Check the new triplet formed by the neighboring arcs
            // to the right for potential circle events.
            it_last++;
            if (it_last != beach_line_.end()) {
                it_last->second.deactivate_circle_event();
                const site_event_type &site_r1 = it_last->first.right_site();
                activate_circle_event(site1, site3, site_r1, it_last);
            }
        }

        // Insert new nodes into the beach line. Update the output.
        beach_line_iterator insert_new_arc(const site_event_type &site_arc1,
                                           const site_event_type &site_arc2,
                                           const site_event_type &site_event,
                                           const beach_line_iterator &position) {
            // Create two new bisectors with opposite directions.
            key_type new_left_node(site_arc1, site_event);
            key_type new_right_node(site_event, site_arc2);

            // Set correct orientation for the first site of the second node.
            if (site_event.is_segment()) {
                new_right_node.inverse_left_site();
            }

            // Update the output.
            edge_type *edge = output_.insert_new_edge(site_arc2, site_event);

            // Update the beach line with the (site_arc1, site_event) bisector.
            beach_line_iterator it = beach_line_.insert(position,
                typename beach_line_type::value_type(new_right_node, value_type(edge->twin())));

            if (site_event.is_segment()) {
                // Update the beach line with temporary bisector, that will
                // disappear after processing site event going through the
                // endpoint of the segment site.
                key_type new_node(site_event, site_event);
                new_node.inverse_right_site();
                beach_line_iterator temp_it = beach_line_.insert(position,
                    typename beach_line_type::value_type(new_node, value_type(NULL)));

                // Update the data structure that holds temporary bisectors.
                end_points_.push(std::make_pair(site_event.point1(), temp_it));
            }

            // Update the beach line with the (site_event, site_arc2) bisector.
            beach_line_.insert(position,
                typename beach_line_type::value_type(new_left_node, value_type(edge)));
            return it;
        }

        // Create a circle event from the given three sites.
        // Returns true if the circle event exists, else false.
        // If exists circle event is saved into the c_event variable.
        bool create_circle_event(const site_event_type &site1,
                                 const site_event_type &site2,
                                 const site_event_type &site3,
                                 circle_event_type &c_event) const {
            if (!site1.is_segment()) {
                if (!site2.is_segment()) {
                    if (!site3.is_segment()) {
                        // (point, point, point) sites.
                        return create_circle_event_ppp(site1, site2, site3, c_event);
                    } else {
                        // (point, point, segment) sites.
                        return create_circle_event_pps(site1, site2, site3, 3, c_event);
                    }
                } else {
                    if (!site3.is_segment()) {
                        // (point, segment, point) sites.
                        return create_circle_event_pps(site1, site3, site2, 2, c_event);
                    } else {
                        // (point, segment, segment) sites.
                        return create_circle_event_pss(site1, site2, site3, 1, c_event);
                    }
                }
            } else {
                if (!site2.is_segment()) {
                    if (!site3.is_segment()) {
                        // (segment, point, point) sites.
                        return create_circle_event_pps(site2, site3, site1, 1, c_event);
                    } else {
                        // (segment, point, segment) sites.
                        return create_circle_event_pss(site2, site1, site3, 2, c_event);
                    }
                } else {
                    if (!site3.is_segment()) {
                        // (segment, segment, point) sites.
                        return create_circle_event_pss(site3, site1, site2, 3, c_event);
                    } else {
                        // (segment, segment, segment) sites.
                        return create_circle_event_sss(site1, site2, site3, c_event);
                    }
                }
            }
        }

        // Add a new circle event to the event queue.
        // bisector_node corresponds to the (site2, site3) bisector.
        void activate_circle_event(const site_event_type &site1,
                                   const site_event_type &site2,
                                   const site_event_type &site3,
                                   beach_line_iterator bisector_node) {
            circle_event_type c_event;
            // Check if the three input sites create a circle event.
            if (create_circle_event(site1, site2, site3, c_event)) {
                // Update circle event's bisector iterator to point to the
                // second bisector that forms it in the beach line.
                c_event.bisector(bisector_node);

                // Add the new circle event to the circle events queue.
                // Update bisector's circle event iterator to point to the
                // new circle event in the circle event queue.
                bisector_node->second.activate_circle_event(
                    circle_events_.push(c_event));
            }
        }

    private:
        struct end_point_comparison {
            bool operator() (const end_point_type &end1, const end_point_type &end2) const {
                return end1.first > end2.first;
            }
        };

        std::vector<site_event_type> site_events_;
        site_event_iterator_type site_event_iterator_;
        std::priority_queue< end_point_type, std::vector<end_point_type>,
                             end_point_comparison > end_points_;
        circle_event_queue_type circle_events_;
        beach_line_type beach_line_;
        output_type &output_;

        //Disallow copy constructor and operator=
        voronoi_builder(const voronoi_builder&);
        void operator=(const voronoi_builder&);
    };

} // sweepline
} // boost
} // detail

#endif

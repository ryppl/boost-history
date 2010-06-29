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

    template <typename Point2D>
    struct beach_line_node;

    template <typename Point2D>
    struct beach_line_node_data;

    template <typename BeachLineNode>
    struct node_comparer;

    // Site event type. 
    // Occurs when sweepline sweeps over one of the initial sites.
    // Contains index of a site below the other sorted sites.
    template <typename Point2D>
    struct site_event {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;

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

    template <typename Point2D>
    site_event<Point2D> make_site_event(typename Point2D::coordinate_type x,
                                        typename Point2D::coordinate_type y,
                                        int index) {
        return site_event<Point2D>(x, y, index);
    }

    template <typename Point2D>
    site_event<Point2D> make_site_event(const Point2D &point, int index) {
        return site_event<Point2D>(point, index);
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
    template <typename Point2D>
    struct circle_event {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef beach_line_node<Point2D> Key;
        typedef beach_line_node_data<typename Point2D::Edge> Value;
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
        int compare(const site_event<Point2D> &s_event) const {
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

        void set_sites(const site_event<Point2D> site1,
                       const site_event<Point2D> site2,
                       const site_event<Point2D> site3) {
            sites_[0] = site1;
            sites_[1] = site2;
            sites_[2] = site3;
        }

        const beach_line_iterator &get_bisector() const {
            return bisector_node_;
        }

        const site_event<Point2D>* get_sites() const {
            return sites_;
        }

    private:
        Point2D center_;
        coordinate_type sqr_radius_;
        beach_line_iterator bisector_node_;
        site_event<Point2D> sites_[3];
    };

    template <typename Point2D>
    circle_event<Point2D> make_circle_event(
        typename Point2D::coordinate_type c_x,
        typename Point2D::coordinate_type c_y,
        typename Point2D::coordinate_type sqr_radius) {
        return circle_event<Point2D>(c_x, c_y, sqr_radius);
    }

    template <typename Point2D>
    circle_event<Point2D> make_circle_event(Point2D center,
        typename Point2D::coordinate_type sqr_radius) {
        return circle_event<Point2D>(center, sqr_radius);
    }

    ///////////////////////////////////////////////////////////////////////////
    // VORONOI CIRCLE EVENTS QUEUE ////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    // Event queue data structure, processes circle events.
    template <typename Point2D>
    class circle_events_queue {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef circle_event<Point2D> circle_event_type;

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
        typedef site_event<Point2D> site_event_type;

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
    template <typename Edge>
    struct beach_line_node_data {
        Edge *edge;

        explicit beach_line_node_data(Edge *new_edge) : edge(new_edge) {}
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

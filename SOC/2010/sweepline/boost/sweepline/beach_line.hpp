// Boost sweepline/beach_line.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <map>
#include <vector>
#include <cmath>

#ifndef BOOST_SWEEPLINE_BEACH_LINE
#define BOOST_SWEEPLINE_BEACH_LINE

namespace boost {
namespace sweepline {

    // Represents bisector made by two arcs that correspond to the left and
    // right points. Arc is defined as curve with points equidistant from the
    // point and from the sweepline.
    // Let sweepline sweep from left to right and it's current coordinate
    // be x0, point coordinates be (x1, y1). In this case equation of the arc
    // may be written as: (x-x0)^2 = (x-x1)^2 + (y-y1)^2, or
    // x = ((y - y1)^2 + x1^2 - x0^2) / (2*(x1 - x0)).
    // In general case two arcs will create two different bisectors. That's why
    // the order of arcs is important to define unique bisector.
    template <typename Point2D>
    struct beach_line_node {
    public:
        typedef typename Point2D Point2D;
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;

        beach_line_node() {}

        // Constructs degenerate bisector, used to search arc that is above
        // given point. The input to the constructor is the site event point.
        explicit beach_line_node(const Point2D &new_point) {
            left_point_ = new_point;
            right_point_ = new_point;
        }

        // Constructs new bisector. The input to the constructor is two points
        // which create bisector. The order of points is important.
        beach_line_node(const Point2D &left_point,
                        const Point2D &right_point) {
            left_point_ = left_point;
            right_point_ = right_point;
        }

        // Returns left point of the bisector.
        const Point2D &get_left_point() const {
            return left_point_;
        }

        // Returns right point of the bisector.
        const Point2D &get_right_point() const {
            return right_point_;
        }

        // Returns x coordinate of the rightmost point.
        coordinate_type get_sweepline_coord() const {
            return std::max(left_point_.x(), right_point_.x());
        }

        // Returns rightmost point.
        const Point2D& get_new_point() const {
            if (left_point_.x() > right_point_.x())
                return left_point_;
            return right_point_;
        }

        // Returns intersection point of the given arc with horizontal line
        // going through new_point. If use_right_point is true we look for the
        // intersection with right arc of the bisector, else with left arc.
        Point2D get_intersection_point(bool use_right_point,
                                       const Point2D &new_point) const {
            const Point2D &use_point = (use_right_point) ?
                                        right_point_ : left_point_;
            coordinate_type vertex_x = ((new_point.y() - use_point.y()) *
                                        (new_point.y() - use_point.y()) /
                                        (use_point.x() - new_point.x()) +
                                         use_point.x() + new_point.x()) *
                                         static_cast<coordinate_type>(0.5);
            return make_point_2d(vertex_x, new_point.y());
        }

        // Returns true if horizontal line going through new_point intersects
        // right arc at first, else returns false. Used during nodes
        // comparison.
        // Let x0 be sweepline coordinate, left point coordinates be (x1, y1),
        // right point coordinates be (x2, y2). Equations of the arcs will be:
        // x1(y) = ((y - y1)^2 + x1^2 - x0^2) / (2*(x1 - x0));
        // x2(y) = ((y - y2)^2 + x2^2 - x0^2) / (2*(x2 - x0)).
        // Horizontal line going throught point (x*, y*) intersects second arc
        // at first if x2(y*) > x1(y*) or:
        // (x2-x0)*(x1-x0)*(x1-x2) + (x2-x0)*(y*-y1)^2 < (x1-x0)*(y*-y2)^2
        bool less(const Point2D &new_point) const {
            coordinate_type sweepline_coord = new_point.x();
            coordinate_type new_node_coord = new_point.y();
            coordinate_type a1 = left_point_.x() - sweepline_coord;
            coordinate_type a2 = right_point_.x() - sweepline_coord;
            coordinate_type b1 = new_node_coord - left_point_.y();
            coordinate_type b2 = new_node_coord - right_point_.y();
            coordinate_type c = left_point_.x() - right_point_.x();
            return a1 * a2 * c + a2 * b1 * b1 < a1 * b2 * b2;
        }

    private:
        Point2D left_point_;
        Point2D right_point_;
    };

    template <typename Point2D>
    struct beach_line_node_data {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;

        beach_line_node_data() {}
    private:
    };

    template <typename BeachLineNode>
    struct node_comparer {
    public:
        typedef typename BeachLineNode::coordinate_type coordinate_type;

        // Compares nodes in the balanced binary search tree. Nodes are
        // compared based on the y coordinates of the arcs intersection points.
        // Nodes with lesser y coordinate go first.
        bool operator() (const BeachLineNode &node1,
                         const BeachLineNode &node2) const {
            // Get x coordinate of the righmost point from both nodes.
            coordinate_type node1_line = node1.get_sweepline_coord();
            coordinate_type node2_line = node2.get_sweepline_coord();

            // Both nodes are situated on the sweepline.
            if (node1_line == node2_line) {
                // Let A be the new site event point, and B the point that
                // creates arc above A. In this case two new nodes are being
                // inserted: (A,B) and (B,A). As intersection points for the
                // first node and for the second are the same we need to
                // compare them based on some another characteristic.
                // That's why we assume that node (C, D) goes before node
                // (D, C), only if D is a site event.
                if (node1.get_left_point() == node2.get_right_point() &&
                    node1.get_right_point() == node2.get_left_point())
                    return node1.get_right_point().x() == node1_line;
                return node1.get_left_point().y() <=
                       node2.get_left_point().y();
            }
            else if (node1_line < node2_line)
                return node1.less(node2.get_new_point());
            else
                return !node2.less(node1.get_new_point());
        }
    };

    // Beach line data structure. Sweepline sweeps from left to right.
    template <typename Key,
              typename Value,
              typename NodeComparer,
              typename EventQueue>
    class beach_line {
    public:
        typedef typename Key::Point2D Point2D;
        typedef typename Key::coordinate_type coordinate_type;
        typedef typename Key::site_event_type site_event_type;
        typedef typename Key::circle_event_type circle_event_type;
        typedef typename std::map< Key, Value, node_comparer<Key> >::const_iterator beach_line_iterator;
        typedef typename std::pair< beach_line_iterator, bool > beach_line_pair;

        // Functor to process events from the event queue.
        struct event_processor {
            explicit event_processor(beach_line &b_line)
                : beach_line_(b_line) {} 

            void operator()(const site_event_type &site_event) {
                beach_line_.process_site_event(site_event);
            }

            void operator()(const circle_event_type &circle_event) {
                beach_line_.process_circle_event(circle_event);
            }

            beach_line &beach_line_;
        };

        beach_line() : event_processor_(*this) {}

        // Init beach line before sweepline run.
        // In case of a few first sites situated on the same
        // vertical line, we init sweepline with all of them.
        // In other case just use the first two sites for the initialization.
        void init(const std::vector<Point2D> &sites) {
            std::sort(sites.begin(), sites.end());
            int skip = 0;

            if (sites.size() == 1) {
                skip = 1;
            } else {
                std::vector<Point2D>::const_iterator it = sites.begin();
                while(it != sites.end() && it->x() == sites.begin()->x()) {
                    it++;
                    skip++;
                }

                if (num == 1) {
                    // Init beach line with two sites.
                    init_beach_line(*sites.begin(), *it);
                } else {
                    // Init beach line with sites situated on the same vertical line.
                    init_beach_line(sites.begin(), it);
                }
            }
            // Init event queue with the rest of the sites.
            event_queue_.init(sites, skip);
        }

        void reset() {
            event_queue_.reset();
            beach_line_.clear();
        }

        void run_sweepline() {
            // Algorithm stops when there are no events in the queue.
            while (!event_queue_.empty()) {
                event_queue_.process_top_event(event_processor_);
                event_queue_.pop();
            }
        }

        void process_site_event(const site_event_type &site_event) {
            // Find the node in the binary search tree with left arc
            // lying above the new site point.
            Key new_key(site_event.get_point());
            beach_line_iterator it = beach_line_.lower_bound(new_key);

            Point2D point_arc;
            Point2D voronoi_vertex;
            if (it == beach_line_.end()) {
                it--;
                point_arc = it->first.get_right_point();
                voronoi_vertex = it->first.get_intersection_point(true, site_event.get_point());

                // Add candidate circle to the event queue.
                activate_circle_event(it->first.get_left_point(),
                                    it->first.get_right_point(),
                                    site_event.get_point());
            } else if (it == beach_line_.begin()) {
                point_arc = it->first.get_left_point();
                voronoi_vertex = it->first.get_intersection_point(false, site_event.get_point());

                // Add candidate circle to the event queue.
                activate_circle_event(site_event.get_point(),
                                    it->first.get_left_point(),
                                    it->first.get_right_point());
            } else {
                point_arc = it->first.get_left_point();
                voronoi_vertex = it->first.get_intersection_point(false, site_event.get_point());

                const Point2D &point2 = it->first.get_left_point();
                const Point2D &point3 = it->first.get_right_point();
                it--;
                const Point2D &point1 = it->first.get_right_point();
                
                // Remove candidate circle from the event queue.
                deactivate_circle_event(point1, point2, point3);

                // Add candidate circles to the event queue.
                activate_circle_event(point1, point2, site_event.get_point());
                activate_circle_event(site_event.get_point(), point2, point3);
            }

            // Create two new nodes.
            Key new_left_node(point_arc, site_event.get_point());
            Key new_right_node(site_event.get_point(), point_arc);
            
            // Insert two new nodes into the binary search tree.
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value()));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value()));
        }

        void process_circle_event(const circle_event_type &circle_event) {
            // Find the node that corresponds to the given circle event.
            Key new_key(circle_event.get_bisector_left_point(),
                        circle_event.get_bisector_right_point());
            beach_line_iterator it_first = beach_line_.find(new_key);
            beach_line_iterator it_last = it_first;

            if (it_first == beach_line_.end())
                return;

            // This will be the second and the third sites that create
            // given circle event.
            Point2D point2 = it_first->first.get_left_point();
            Point2D point3 = it_first->first.get_right_point();
            it_first--;
            it_last++;
            // This will be the first site that creates given circle event.
            Point2D point1 = it_first->first.get_left_point();

            // Delete nodes that correspond to the (1st and 2d points) and
            // (2d and 3d points).
            beach_line_.erase(it_first, it_last);

            // Insert new node that corresponds to the (1st and 3d points).
            Key new_node(point1, point3);
            beach_line_pair it_pair = 
                beach_line_.insert(std::pair<Key, Value>(new_node, Value()));

            it_first = it_pair.first;
            it_last = it_first;

            // Check the new triplets formed by the neighboring arcs
            // for potential circle events. Check left.
            if (it_first != beach_line_.begin()) {
                it_first--;
                const Point2D &point_l1 = it_first->first.get_left_point();
                deactivate_circle_event(point_l1, point1, point2);
                if (it_first != beach_line_.begin()) {
                    it_first--;
                    const Point2D &point_l2 = it_first->first.get_left_point();
                    activate_circle_event(point_l2, point_l1, point1);
                }
            }

            // Check the new triplets formed by the neighboring arcs
            // for potential circle events. Check right.
            it_last++;
            if (it_last != beach_line_.end()) {
                const Point2D &point_r1 = it_last->first.get_right_point();
                deactivate_circle_event(point2, point3, point_r1);
                it_last++;
                if (it_last != beach_line_.end()) {
                    it_last++;
                    const Point2D &point_r2 = it_last->first.get_right_point();
                    activate_circle_event(point3, point_r1, point_r2);
                }
            }            
        }

    protected:
        void init_beach_line(typename std::vector<Point2D>::const_iterator it_begin,
                             typename std::vector<Point2D>::const_iterator it_end) {
             typename std::vector<Point2D>::const_iterator it_first = it_begin;
             typename std::vector<Point2D>::const_iterator it_second = it_begin;
             it_second++;
             while (it_second != it_end) {
                 beach_line_node new_node(*it_first, *it_second);
                 beach_line_.insert(std::pair<Key, Value>(new_node, Value()));
                 it_first++;
                 it_second++;
             }
        }

        void init_beach_line(const Point2D &first_point,
                             const Point2D &second_point) {
            beach_line_node new_left_node(firs_point, second_point);
            beach_line_node new_second_node(second_point, first_point);
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value()));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value()));
        }

        // Create circle event from given three points.
        bool create_circle_event(const Point2D &point1,
                                 const Point2D &point2,
                                 const Point2D &point3,
                                 circle_event_type &c_event) const {
            coordinate_type a = (point1.x() - point2.x())*
                                (point2.y() - point3.y())-
                                (point1.y() - point2.y())*
                                (point2.x() - point3.x());
            // Check if bisectors intersect.
            if (a == static_cast<coordinate_type>(0))
                return false;
            coordinate_type b1 = (point1.x() - point2.x())*
                                 (point1.x() + point2.x())+
                                 (point1.y() - point2.y())*
                                 (point1.y() + point2.y());
            coordinate_type b2 = (point2.x() - point3.x())*
                                 (point2.x() + point3.x())+
                                 (point2.y() - point3.y())*
                                 (point2.y() + point3.y());
            coordinate_type c_x = (b1*(point2.y() - point3.y()) -
                                   b2*(point1.y() - point2.y())) / a *
                                  static_cast<coordinate_type>(0.5);
            coordinate_type c_y = (b2*(point1.x() - point2.x()) -
                                   b1*(point2.x() - point3.x())) / a *
                                   static_cast<coordinate_type>(0.5);
            coordinate_type sqr_radius = (c_x-point1.x())*(c_x-point1.x()) +
                                         (c_y-point1.y())*(c_y-point1.y());
            // Create new circle event;
            c_event = make_circle_event(c_x, c_y, sqr_radius);
            c_event.set_bisector(point2, point3);
            return true;
        }

        // Add new circle event to the event queue.
        void activate_circle_event(const Point2D &point1,
                                   const Point2D &point2,
                                   const Point2D &point3) {
            circle_event_type c_event;
            if (create_circle_event(point1, point2, point3, c_event))
                event_queue_.push(c_event);
        }

        // Remove circle event from the event queue.
        void deactivate_circle_event(const Point2D &point1,
                                     const Point2D &point2,
                                     const Point2D &point3) {
            circle_event_type c_event;
            if (create_circle_event(point1, point2, point3, c_event))
                event_queue_.deactivate_event(c_event);
        }

    private:
        EventQueue event_queue_;
        event_processor event_processor_;
        std::map< Key, Value, NodeComparer > beach_line_;
    };

} // sweepline
} // boost

#endif
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
        typedef typename Point2D Point2D;
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

        // Returns x coordinate of the rightmost site.
        coordinate_type get_sweepline_coord() const {
            return std::max(left_site_.x(), right_site_.x());
        }

        // Returns rightmost site.
        const site_event_type& get_new_site() const {
            if (left_site_.x() > right_site_.x())
                return left_site_;
            return right_site_;
        }

        // Returns true if horizontal line going through new site intersects
        // right arc at first, else returns false. Used during nodes
        // comparison.
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
        beach_line_node_data(Edge &new_edge) : edge(new_edge) {}

        Edge &edge;
    };

    template <typename BeachLineNode>
    struct node_comparer {
    public:
        typedef typename BeachLineNode::coordinate_type coordinate_type;

        // Compares nodes in the balanced binary search tree. Nodes are
        // compared based on the y coordinates of the arcs intersection points.
        // Nodes with lesser y coordinate of the intersection point go first.
        bool operator() (const BeachLineNode &node1,
                         const BeachLineNode &node2) const {
            // Get x coordinate of the righmost site from both nodes.
            coordinate_type node1_line = node1.get_sweepline_coord();
            coordinate_type node2_line = node2.get_sweepline_coord();

            // Both nodes are situated on the same vertical line.
            if (node1_line == node2_line) {
                // Let A be the new site event point, and B the site that
                // creates arc above A. In this case two new nodes are being
                // inserted: (A,B) and (B,A). As intersection points for the
                // first node and for the second are the same we need to
                // compare them based on some another characteristic.
                // That's why we assume that node (C, D) goes before node
                // (D, C), only if D lies on the sweepline.
                if (node1.get_left_site() == node2.get_right_site() &&
                    node1.get_right_site() == node2.get_left_site())
                    return node1.get_right_site().x() == node1_line;

                // Used when we are searching for the bisector during
                // circle events processing. Guarantees correct comparison.
                if (node1.get_left_site() == node2.get_left_site() &&
                    node1.get_right_site() == node2.get_right_site())
                    return false;

                return node1.get_left_site().y() <=
                       node2.get_left_site().y();
            }
            else if (node1_line < node2_line)
                return node1.less(node2.get_new_site());
            else
                return !node2.less(node1.get_new_site());
        }
    };

    // Beach line data structure. Sweepline sweeps from left to right.
    template <typename Key,
              typename Value,
              typename NodeComparer,
              typename EventQueue,
              typename Output>
    class beach_line {
    public:
        typedef typename Key::Point2D Point2D;
        typedef typename Key::coordinate_type coordinate_type;
        typedef typename Key::site_event_type site_event_type;
        typedef typename Key::circle_event_type circle_event_type;

        typedef typename Output::edge_type edge_type;

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

        beach_line() : event_processor_(*this) {
        }

        // Init beach line before sweepline run.
        // In case of a few first sites situated on the same
        // vertical line, we init beach line with all of them.
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
            output_.init(sites.size());
        }

        void reset() {
            event_queue_.reset();
            beach_line_.clear();
            output_.clear();
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
            Key new_key(site_event);
            beach_line_iterator it = beach_line_.lower_bound(new_key);

            site_event_type site_arc;
            if (it == beach_line_.end()) {
                it--;
                site_arc = it->first.get_right_site();

                // Add candidate circle to the event queue.
                activate_circle_event(it->first.get_left_site(),
                                      it->first.get_right_site(),
                                      site_event);
            } else if (it == beach_line_.begin()) {
                site_arc = it->first.get_left_site();

                // Add candidate circle to the event queue.
                activate_circle_event(site_event,
                                      it->first.get_left_site(),
                                      it->first.get_right_site());
            } else {
                site_arc = it->first.get_left_site();
                const site_event_type &site2 = it->first.get_left_site();
                const site_event_type &site3 = it->first.get_right_site();
                it--;
                const site_event_type &site1 = it->first.get_right_site();
                
                // Remove candidate circle from the event queue.
                deactivate_circle_event(site1, site2, site3);

                // Add candidate circles to the event queue.
                activate_circle_event(site1, site2, site_event);
                activate_circle_event(site_event, site2, site3);
            }

            // Create two new nodes.
            Key new_left_node(site_arc, site_event);
            Key new_right_node(site_event, site_arc);
            int site_index1 = site_arc.get_site_index();
            int site_index2 = site_event.get_site_index();
            
            // Insert two new nodes into the binary search tree.
            // Update output.
            edge_type &edge = output_.insert_new_edge(site_arc, site_event);
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value(edge)));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value(edge)));
        }

        void process_circle_event(const circle_event_type &circle_event) {
            // Find the node that corresponds to the given circle event.            
            Key new_key(circle_event.get_bisector_left_site(),
                        circle_event.get_bisector_right_site());
            beach_line_iterator it_first = beach_line_.find(new_key);
            beach_line_iterator it_last = it_first;

            // Get the second and the third sites that create given circle event.
            site_event_type site2 = it_first->first.get_left_site();
            site_event_type site3 = it_first->first.get_right_site();

            // Get second bisector;
            Value bisector2 = it_first->second;
            
            // Get first bisector;
            it_first--;
            Value bisector1 = it_first->second;
            
            // Get the first site that creates given circle event.
            site_event_type site1 = it_first->first.get_left_site();

            // Delete nodes that correspond to the (1st and 2d points) and
            // (2d and 3d points).
            it_last++;
            beach_line_.erase(it_first, it_last);

            // Insert new node that corresponds to the (1st and 3d points).
            // Update output.
            Key new_node(site1, site3);
            beach_line_pair it_pair = beach_line_.insert(std::pair<Key, Value>(
                new_node,
                output_.insert_new_edge(site1, site2, site3,
                                        circle_event,
                                        bisector1.edge, bisector2.edge)));
            it_first = it_pair.first;
            it_last = it_first;

            // Check the new triplets formed by the neighboring arcs
            // for potential circle events. Check left.
            if (it_first != beach_line_.begin()) {
                it_first--;
                const site_event_type &site_l1 = it_first->first.get_left_site();
                deactivate_circle_event(site_l1, site1, site2);
                if (it_first != beach_line_.begin()) {
                    it_first--;
                    const site_event_type &site_l2 = it_first->first.get_left_site();
                    activate_circle_event(site_l2, site_l1, site1);
                }
            }

            // Check the new triplets formed by the neighboring arcs
            // for potential circle events. Check right.
            it_last++;
            if (it_last != beach_line_.end()) {
                const site_event_type &site_r1 = it_last->first.get_right_site();
                deactivate_circle_event(site2, site3, site_r1);
                it_last++;
                if (it_last != beach_line_.end()) {
                    it_last++;
                    const site_event_type &site_r2 = it_last->first.get_right_site();
                    activate_circle_event(site3, site_r1, site_r2);
                }
            }            
        }

    protected:
        void init_beach_line(typename std::vector<Point2D>::const_iterator it_begin,
                             typename std::vector<Point2D>::const_iterator it_end) {
             typename std::vector<Point2D>::const_iterator it_first = it_begin;
             typename std::vector<Point2D>::const_iterator it_second = it_begin;
             it_second++;
             int cur_site = 0;
             while (it_second != it_end) {
                 site_event_type site1 = make_site_event(it_first->x, it_first->y, cur_site);
                 site_event_type site2 = make_site_event(it_second->x, it_second->y, cur_site+1);

                 // Create new beach line node.
                 beach_line_node new_node(site1, site2);
                 
                 // Update output.
                 edge_type &edge = output_.insert_new_edge(site1, site2);

                 // Insert new node into the binary search tree.
                 beach_line_.insert(std::pair<Key, Value>(new_node, Value(edge)));
                 
                 // Update iterators.
                 it_first++;
                 it_second++;
                 cur_site++;
             }
        }

        void init_beach_line(const Point2D &first_point,
                             const Point2D &second_point) {
            site_event_type site1 = make_site_event(first_point.x(), first_point.y(), 0);
            site_event_type site2 = make_site_event(second_point.x(), second_point.y(), 1);

            // Create two new beach line nodes.
            beach_line_node new_left_node(site1, site2);
            beach_line_node new_second_node(site2, site1);

            // Update output.
            edge_type &edge = output_.insert_new_edge(site1, site2);

            // Insert two new nodes into the binary search tree.
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value(edge)));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value(egge)));
        }

        // Create circle event from the given three points.
        bool create_circle_event(const site_event_type &site1,
                                 const site_event_type &site2,
                                 const site_event_type &site3,
                                 circle_event_type &c_event) const {
            coordinate_type a = (site1.x() - site2.x())*
                                (site2.y() - site3.y())-
                                (site1.y() - site2.y())*
                                (site2.x() - site3.x());
            // Check if bisectors intersect.
            if (a <= static_cast<coordinate_type>(0))
                return false;
            coordinate_type b1 = (site1.x() - site2.x())*
                                 (site1.x() + site2.x())+
                                 (site1.y() - site2.y())*
                                 (site1.y() + site2.y());
            coordinate_type b2 = (site2.x() - site3.x())*
                                 (site2.x() + site3.x())+
                                 (site2.y() - site3.y())*
                                 (site2.y() + site3.y());
            coordinate_type c_x = (b1*(site2.y() - site3.y()) -
                                   b2*(site1.y() - site2.y())) / a *
                                  static_cast<coordinate_type>(0.5);
            coordinate_type c_y = (b2*(site1.x() - site2.x()) -
                                   b1*(site2.x() - site3.x())) / a *
                                   static_cast<coordinate_type>(0.5);
            coordinate_type sqr_radius = (c_x-site1.x())*(c_x-site1.x()) +
                                         (c_y-site1.y())*(c_y-site1.y());
            // Create new circle event;
            c_event = make_circle_event(c_x, c_y, sqr_radius);
            c_event.set_bisector(site2, site3);
            return true;
        }

        // Add new circle event to the event queue.
        void activate_circle_event(const site_event_type &site1,
                                   const site_event_type &site2,
                                   const site_event_type &site3) {
            circle_event_type c_event;
            if (create_circle_event(site1, site2, site3, c_event))
                event_queue_.push(c_event);
        }

        // Remove circle event from the event queue.
        void deactivate_circle_event(const site_event_type &point1,
                                     const site_event_type &point2,
                                     const site_event_type &point3) {
            circle_event_type c_event;
            if (create_circle_event(point1, point2, point3, c_event))
                event_queue_.deactivate_event(c_event);
        }

    private:
        EventQueue event_queue_;
        event_processor event_processor_;
        std::map< Key, Value, NodeComparer > beach_line_;
        Output output_;
    };

} // sweepline
} // boost

#endif
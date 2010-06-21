// Boost sweepline/voronoi_formation.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <map>
#include <vector>

#include "event_queue.hpp"
#include "event_types.hpp"
#include "voronoi_output.hpp"

#ifndef BOOST_SWEEPLINE_VORONOI_FORMATION
#define BOOST_SWEEPLINE_VORONOI_FORMATION

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

            // Both nodes are situated on the same vertical line.
            if (node1_line == node2_line) {
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
            else if (node1_line < node2_line)
                return node1.less(node2.get_new_site());
            else
                return !node2.less(node1.get_new_site());
        }
    };

    // Voronoi diagram formation. Sweepline sweeps from left to right.
    template <typename T>
    class voronoi_formation {
    public:
        typedef typename point_2d<T> Point2D;
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;

        typedef typename voronoi_output<Point2D> Output;
        typedef typename Output::edge_type edge_type;
        typedef typename Output::edge_iterator edge_iterator;
        typedef typename Output::cell_records cell_records;
        typedef typename Output::voronoi_vertices voronoi_vertices;
        typedef typename voronoi_vertices::const_iterator voronoi_vertices_iterator;

        typedef typename event_queue<Point2D> EventQueue;
        typedef typename beach_line_node<Point2D> Key;
        typedef typename beach_line_node_data<Point2D> Value;
        typedef typename node_comparer<Key> NodeComparer;
        typedef typename std::map< Key, Value, NodeComparer > BeachLine;
        typedef typename BeachLine::const_iterator beach_line_iterator;

        // Functor to process events from the event queue.
        struct event_processor {
            event_processor() : beach_line_(NULL) {} 

            void operator()(const site_event_type &site_event) {
                beach_line_->process_site_event(site_event);
            }

            void operator()(const circle_event_type &circle_event) {
                beach_line_->process_circle_event(circle_event);
            }

            voronoi_formation *beach_line_;
        };

        voronoi_formation() : event_processor_() {}

        // Init beach line before sweepline run.
        // In case of a few first sites situated on the same
        // vertical line, we init beach line with all of them.
        // In other case just use the first two sites for the initialization.
        void init(std::vector<Point2D> &sites) {
            output_.init(sites.size());

            // Init beach_line pointer in the event_processor data sturcture.
            // This is done here to avoid compiler warning in the constructor.
            event_processor_.beach_line_ = this;
            
            // Sort all sites.
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

                if (skip == 1) {
                    // Init beach line with two sites.
                    init_beach_line(*sites.begin(), *it);

                    // Skip the second point also.
                    skip++;
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
            output_.clear();
        }

        void run_sweepline() {
            // Algorithm stops when there are no events in the queue.
            while (!event_queue_.empty()) {
                event_queue_.process_top_event(event_processor_);
                event_queue_.pop();
            }
        }

        // Uses special comparison function for the lower bound and insertion
        // operations.
        void process_site_event(const site_event_type &site_event) {
            // Find the node in the binary search tree with left arc
            // lying above the new site point.
            Key new_key(site_event);
            beach_line_iterator it = beach_line_.lower_bound(new_key);

            site_event_type site_arc;
            if (it == beach_line_.end()) {
                it--;
                site_arc = it->first.get_right_site();

                // Insert new arc into the sweepline.
                beach_line_iterator new_node_it = insert_new_arc(site_arc, site_event);
                new_node_it--;

                // Add candidate circle to the event queue.
                activate_circle_event(it->first.get_left_site(),
                                      it->first.get_right_site(),
                                      site_event,
                                      new_node_it);
            } else if (it == beach_line_.begin()) {
                site_arc = it->first.get_left_site();

                // Insert new arc into the sweepline.
                beach_line_iterator new_node_it = insert_new_arc(site_arc, site_event);
                new_node_it++;

                // Add candidate circle to the event queue.
                activate_circle_event(site_event,
                                      it->first.get_left_site(),
                                      it->first.get_right_site(),
                                      new_node_it);
            } else {
                site_arc = it->first.get_left_site();

                // Insert new arc into the sweepline.
                beach_line_iterator new_node_it = insert_new_arc(site_arc, site_event);

                const site_event_type &site2 = it->first.get_left_site();
                const site_event_type &site3 = it->first.get_right_site();
                it--;
                const site_event_type &site1 = it->first.get_right_site();

                // Remove candidate circle from the event queue.
                deactivate_circle_event(site1, site2, site3);

                // Add candidate circles to the event queue.
                new_node_it--;
                activate_circle_event(site1, site2, site_event, new_node_it);
                new_node_it++;
                new_node_it++;
                activate_circle_event(site_event, site2, site3, new_node_it);
            }               
        }

        // Doesn't use special comparison function as it works fine only for
        // the site events processing.
        void process_circle_event(const circle_event_type &circle_event) {
            // Retrieve the second bisector node that corresponds to the given
            // circle event.
            beach_line_iterator it_first = circle_event.get_bisector();
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

            // Let circle event sites be A, B, C, two bisectors that define
            // circle event be (A, B), (B, C). During circle event processing
            // we remove (A, B), (B, C) and insert (A, C). As beach line nodes 
            // comparer doesn't work fine for the circle events. We only remove
            // (B, C) bisector and change (A, B) bisector to the (A, C). That's
            // why we use const_cast there and take all the responsibility that
            // map data structure keeps correct ordering.
            const_cast<Key &>(it_first->first).set_right_site(it_last->first.get_right_site());
            edge_type *edge = output_.insert_new_edge(site1, site2, site3, circle_event, bisector1.edge, bisector2.edge);
            const_cast<Value &>(it_first->second).change_edge(edge);
            beach_line_.erase(it_last);
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
                    it_first++;
                    activate_circle_event(site_l2, site_l1, site1, it_first);
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
                    activate_circle_event(site3, site_r1, site_r2, it_last);
                }
            }            
        }

        const cell_records &get_cell_records() const {
            return output_.get_cell_records();
        }

        const voronoi_vertices &get_voronoi_vertices() const {
            return output_.get_voronoi_vertices();
        }

    protected:
        void init_beach_line(typename std::vector<Point2D>::const_iterator it_begin,
                             typename std::vector<Point2D>::const_iterator it_end) {
             typename std::vector<Point2D>::const_iterator it_first = it_begin;
             typename std::vector<Point2D>::const_iterator it_second = it_begin;
             it_second++;
             int cur_site = 0;
             while (it_second != it_end) {
                 site_event_type site1 = make_site_event<coordinate_type>(
                     it_first->x(), it_first->y(), cur_site);
                 site_event_type site2 = make_site_event<coordinate_type>(
                     it_second->x(), it_second->y(), cur_site+1);

                 // Create new beach line node.
                 Key new_node(site1, site2);
                 
                 // Update output.
                 edge_type *edge = output_.insert_new_edge(site1, site2);

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
            site_event_type site1 = make_site_event<coordinate_type>(
                first_point.x(), first_point.y(), 0);
            site_event_type site2 = make_site_event<coordinate_type>(
                second_point.x(), second_point.y(), 1);

            // Create two new beach line nodes.
            Key new_left_node(site1, site2);
            Key new_right_node(site2, site1);

            // Update output.
            edge_type *edge = output_.insert_new_edge(site1, site2);

            // Insert two new nodes into the binary search tree.
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value(edge)));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value(edge->twin)));
        }

        // Insert new arc below site arc into the beach line.
        beach_line_iterator insert_new_arc(const site_event_type &site_arc,
                                           const site_event_type &site_event) {
            // Create two new nodes.
            Key new_left_node(site_arc, site_event);
            Key new_right_node(site_event, site_arc);
            int site_index1 = site_arc.get_site_index();
            int site_index2 = site_event.get_site_index();
            
            // Insert two new nodes into the binary search tree.
            // Update output.
            edge_type *edge = output_.insert_new_edge(site_arc, site_event);
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value(edge)));
            return beach_line_.insert(std::pair<Key, Value>(new_right_node, Value(edge->twin))).first;
        }

        // Create circle event from the given three points.
        bool create_circle_event(const site_event_type &site1,
                                 const site_event_type &site2,
                                 const site_event_type &site3,
                                 circle_event_type &c_event) const {
            coordinate_type a = (site1.x() - site2.x()) * (site2.y() - site3.y()) -
                                (site1.y() - site2.y()) * (site2.x() - site3.x());
            // Check if bisectors intersect.
            if (a >= static_cast<coordinate_type>(0))
                return false;
            coordinate_type b1 = (site1.x() - site2.x()) * (site1.x() + site2.x())+
                                 (site1.y() - site2.y()) * (site1.y() + site2.y());
            coordinate_type b2 = (site2.x() - site3.x()) * (site2.x() + site3.x())+
                                 (site2.y() - site3.y()) * (site2.y() + site3.y());
            coordinate_type c_x = (b1*(site2.y() - site3.y()) - b2*(site1.y() - site2.y())) / a *
                                  static_cast<coordinate_type>(0.5);
            coordinate_type c_y = (b2*(site1.x() - site2.x()) - b1*(site2.x() - site3.x())) / a *
                                   static_cast<coordinate_type>(0.5);
            coordinate_type sqr_radius = (c_x-site1.x())*(c_x-site1.x()) +
                                         (c_y-site1.y())*(c_y-site1.y());
            // Create new circle event;
            c_event = make_circle_event(c_x, c_y, sqr_radius);
            return true;
        }

        // Add new circle event to the event queue.
        void activate_circle_event(const site_event_type &site1,
                                   const site_event_type &site2,
                                   const site_event_type &site3,
                                   beach_line_iterator bisector_node) {
            circle_event_type c_event;
            if (create_circle_event(site1, site2, site3, c_event)) {
                c_event.set_bisector(bisector_node);
                event_queue_.push(c_event);
            }
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
        BeachLine beach_line_;
        Output output_;

        //Disallow copy constructor and operator=
        voronoi_formation(const voronoi_formation&);
        void operator=(const voronoi_formation&);
    };

} // sweepline
} // boost

#endif
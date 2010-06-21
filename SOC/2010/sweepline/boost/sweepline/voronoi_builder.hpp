// Boost sweepline/voronoi_builder.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "detail/voronoi_formation.hpp"

#ifndef BOOST_SWEEPLINE_VORONOI_BUILDER
#define BOOST_SWEEPLINE_VORONOI_BUILDER

namespace boost {
namespace sweepline {
    
    using namespace detail;

    // Voronoi diagram builder. Sweepline sweeps from left to right.
    template <typename T>
    class voronoi_builder {
    public:
        typedef typename point_2d<T> Point2D;
        typedef typename Point2D::coordinate_type coordinate_type;

        typedef typename voronoi_output<Point2D>::edge_type edge_type;
        typedef typename voronoi_output<Point2D>::edge_iterator edge_iterator;
        typedef typename voronoi_output<Point2D>::cell_records cell_records;
        typedef typename voronoi_output<Point2D>::voronoi_vertices voronoi_vertices;
        typedef typename voronoi_vertices::const_iterator voronoi_vertices_iterator;

        voronoi_builder() {}

        // Init beach line before sweepline run.
        // In case of a few first sites situated on the same
        // vertical line, we init beach line with all of them.
        // In other case just use the first two sites for the initialization.
        void init(std::vector<Point2D> &sites) {
            // Sort all sites.
            std::sort(sites.begin(), sites.end());

            // Add all unique sites to the site events vector.
            int site_event_index = 0;
            int sz = sites.size();
            for (int i = 0; i < sz; i++) {
                if (!i || sites[i] != sites[i-1]) {
                    site_events_.push_back(make_site_event(sites[i].x(), sites[i].y(),
                                                          site_event_index));
                    site_event_index++;
                }
            }

            // Init output with number of site events.
            output_.init(site_events_.size());

            // Set sites iterator.
            site_events_iterator_ = site_events_.begin();
            
            int skip = 0;
            if (site_events_.size() == 1) {
                skip = 1;
                site_events_iterator_++;
            } else {
                while(site_events_iterator_ != site_events_.end() &&
                      site_events_iterator_->x() == site_events_.begin()->x()) {
                    site_events_iterator_++;
                    skip++;
                }

                if (skip == 1) {
                    // Init beach line with two sites.
                    init_beach_line();

                    // Skip the second point also.
                    site_events_iterator_++;
                } else {
                    // Init beach line with sites situated on the same vertical line.
                    init_beach_line_colinear_sites();
                }
            }
        }

        void reset() {
            site_events_.clear();
            site_events_iterator_ = site_events_.begin();
            circle_events_.reset();
            beach_line_.clear();
            output_.clear();
        }

        void run_sweepline() {
            // Algorithm stops when there are no events to process.
            while (!circle_events_.empty() || 
                   !(site_events_iterator_ == site_events_.end())) {
                if (circle_events_.empty()) {
                    process_site_event(*site_events_iterator_);
                    site_events_iterator_++;
                } else if (site_events_iterator_ == site_events_.end()) {
                    process_circle_event(circle_events_.top());
                    circle_events_.pop();
                } else {
                    if (circle_events_.top().compare(*site_events_iterator_) >= 0) {
                        process_site_event(*site_events_iterator_);
                        site_events_iterator_++;
                    } else {
                        process_circle_event(circle_events_.top());
                        circle_events_.pop();
                    }
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
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;
        typedef typename std::vector<site_event_type>::const_iterator site_events_iterator;

        typedef typename circle_events_queue<Point2D> CircleEventsQueue;
        typedef typename beach_line_node<Point2D> Key;
        typedef typename beach_line_node_data<Point2D> Value;
        typedef typename node_comparer<Key> NodeComparer;
        typedef typename std::map< Key, Value, NodeComparer > BeachLine;
        typedef typename BeachLine::const_iterator beach_line_iterator;

        typedef typename voronoi_output<Point2D> Output;

        void init_beach_line() {
            site_events_iterator it_first = site_events_.begin();
            site_events_iterator it_second = site_events_.begin();
            it_second++;

            // Create two new beach line nodes.
            Key new_left_node(*it_first, *it_second);
            Key new_right_node(*it_second, *it_first);

            // Update output.
            edge_type *edge = output_.insert_new_edge(*it_first, *it_second);

            // Insert two new nodes into the binary search tree.
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value(edge)));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value(edge->twin)));
        }

        void init_beach_line_colinear_sites() {
             site_events_iterator it_first = site_events_.begin();
             site_events_iterator it_second = site_events_.begin();
             it_second++;
             int cur_site = 0;
             while (it_second != site_events_iterator_) {
                 // Create new beach line node.
                 Key new_node(*it_first, *it_second);
                 
                 // Update output.
                 edge_type *edge = output_.insert_new_edge(*it_first, *it_second);

                 // Insert new node into the binary search tree.
                 beach_line_.insert(std::pair<Key, Value>(new_node, Value(edge)));
                 
                 // Update iterators.
                 it_first++;
                 it_second++;
                 cur_site++;
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
                deactivate_circle_event(site_l1, site1, site3);
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
                deactivate_circle_event(site1, site3, site_r1);
                it_last++;
                if (it_last != beach_line_.end()) {
                    const site_event_type &site_r2 = it_last->first.get_right_site();
                    activate_circle_event(site3, site_r1, site_r2, it_last);
                }
            }            
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
                circle_events_.push(c_event);
            }
        }

        // Remove circle event from the event queue.
        void deactivate_circle_event(const site_event_type &point1,
                                     const site_event_type &point2,
                                     const site_event_type &point3) {
            circle_event_type c_event;
            if (create_circle_event(point1, point2, point3, c_event))
                circle_events_.deactivate_event(c_event);
        }

    private:
        std::vector<site_event_type> site_events_;
        site_events_iterator site_events_iterator_;
        CircleEventsQueue circle_events_;
        BeachLine beach_line_;
        Output output_;

        //Disallow copy constructor and operator=
        voronoi_builder(const voronoi_builder&);
        void operator=(const voronoi_builder&);
    };

} // sweepline
} // boost

#endif
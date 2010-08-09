// Boost sweepline/voronoi_segment_builder.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SEGMENT_BUILDER
#define BOOST_SWEEPLINE_VORONOI_SEGMENT_BUILDER

#include <algorithm>

namespace boost {
namespace sweepline {

    // Voronoi diagram builder. Sweepline sweeps from left to right.
    template <typename T>
    class voronoi_builder {
    public:
        typedef T coordinate_type;
        typedef point_2d<coordinate_type> Point2D;
        typedef std::pair<Point2D, Point2D> Segment2D;
        typedef voronoi_output_clipped<coordinate_type> ClippedOutput;
        typedef typename detail::site_event<coordinate_type> site_event_type;
        typedef typename detail::circle_event<coordinate_type> circle_event_type;

        voronoi_builder() {
            // Set sites iterator.
            site_events_iterator_ = site_events_.begin();
        }

        void init(std::vector<Point2D> &points) {
            std::vector<Segment2D> empty_vec;
            init(points, empty_vec);
        }

        // Init beach line before sweepline run.
        // In case of a few first sites situated on the same
        // vertical line, we init beach line with all of them.
        // In other case just use the first two sites for the initialization.
        void init(std::vector<Point2D> &points, std::vector<Segment2D> &segments) {
            // Clear all data structures.
            reset();

            // Return if there are no sites.
            if (points.empty() && segments.empty()) {
                site_events_iterator_ = site_events_.begin();
                return;
            }

            // TODO(asydorchuk): Add segments intersection check.

            // Avoid additional memory reallocations.
            site_events_.reserve(points.size() + segments.size() * 3);

            // Create site events from point sites.
            for (typename std::vector<Point2D>::const_iterator it = points.begin();
                 it != points.end(); it++) {
                site_events_.push_back(detail::make_site_event(it->x(), it->y(), 0));
            }
            
            // Create site events from end points of segment sites and segment itself.
            for (typename std::vector<Segment2D>::const_iterator it = segments.begin();
                 it != segments.end(); it++) {
                site_events_.push_back(detail::make_site_event(it->first, 0));
                site_events_.push_back(detail::make_site_event(it->second, 0));
                site_events_.push_back(detail::make_site_event(it->first, it->second, 0));
            }

            // Sort site events.
            sort(site_events_.begin(), site_events_.end());

            // Remove duplicates.
            site_events_.erase(unique(site_events_.begin(), site_events_.end()),
                               site_events_.end());

            // Number sites.
            for (int cur_index = 0; cur_index < static_cast<int>(site_events_.size()); cur_index++)
                site_events_[cur_index].set_site_index(cur_index);

            // Set site iterator.
            site_events_iterator_ = site_events_.begin();

            // Init output with number of site events.
            // There will be one site event for each input point and three site events
            // for each input segment: both ends of a segment and the segment itself.
            output_.init(site_events_.size());

            int skip = 0;
            if (site_events_.size() == 1) {
                // Handle one input site case.
                output_.process_single_site(site_events_[0]);
                skip = 1;
                site_events_iterator_++;
            } else {
                // Init beach line.
                while(site_events_iterator_ != site_events_.end() &&
                      site_events_iterator_->x() == site_events_.begin()->x() &&
                      site_events_iterator_->is_vertical()) {
                    site_events_iterator_++;
                    skip++;
                }

                if (skip == 1) {
                    // Init beach line with two sites.
                    init_beach_line();
                } else {
                    // Init beach line with sites situated on the same vertical line.
                    init_beach_line_collinear_sites();
                }
            }
        }

        void reset() {
            output_.reset();
            circle_events_.reset();
            beach_line_.clear();
            site_events_.clear();
            site_events_iterator_ = site_events_.begin();
        }

        void run_sweepline() {
            // Algorithm stops when there are no events to process.
            while (!circle_events_.empty() || 
                   !(site_events_iterator_ == site_events_.end())) {
                if (circle_events_.empty())
                    process_site_event();
                else if (site_events_iterator_ == site_events_.end())
                    process_circle_event();
                else {
                    if (circle_events_.top().compare(*site_events_iterator_) > 0)
                        process_site_event();
                    else
                        process_circle_event();
                }
            }

            // Simplify output.
            output_.simplify();
        }

        // Returns output bounding rectangle that includes all the vertices and sites
        // of the voronoi diagram.
        const BRect<coordinate_type> &get_bounding_rectangle() const {
            return output_.get_bounding_rectangle();
        }

        // Clip using bounding rectangle that includes all the vertices and sites
        // of the voronoi diagram.
        void clip(ClippedOutput &clipped_output) {
            output_.clip(clipped_output);
        }

        // Clip using defined rectangle.
        void clip(const BRect<coordinate_type> &brect, ClippedOutput &clipped_output) {
            output_.clip(brect, clipped_output);
        }

    protected:
        typedef typename std::vector<site_event_type>::const_iterator site_events_iterator_type;
        
        typedef detail::voronoi_output<coordinate_type> Output;
        typedef typename Output::edge_type edge_type;

        typedef typename detail::circle_events_queue<coordinate_type> CircleEventsQueue;
        typedef typename detail::beach_line_node<coordinate_type> Key;
        typedef typename detail::beach_line_node_data<coordinate_type> Value;
        typedef typename detail::node_comparer<Key> NodeComparer;
        typedef std::map< Key, Value, NodeComparer > BeachLine;
        typedef typename std::map< Key, Value, NodeComparer >::iterator beach_line_iterator;

        void init_beach_line() {
            // The first site is always a point.
            // Get the first and the second site events.
            site_events_iterator_type it_first = site_events_.begin();
            site_events_iterator_type it_second = site_events_.begin();
            it_second++;

            // The second site might be a point or a segment.
            // Create two new beach line nodes.
            Key new_left_node(*it_first, *it_second);
            Key new_right_node(*it_second, *it_first);

            // TODO(asydorchuk) change output insertion!!!.
            // Update output.
            edge_type *edge = output_.insert_new_edge(*it_first, *it_second);

            // Insert two new nodes into the binary search tree.
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value(edge)));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value(edge->twin)));

            // The second site has been already processed.
            site_events_iterator_++;
        }

        // Insert bisectors for all collinear initial sites.
        // There should be at least two colinear sites.
        void init_beach_line_collinear_sites() {
             site_events_iterator_type it_first = site_events_.begin();
             site_events_iterator_type it_second = site_events_.begin();
             it_second++;
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
             }
        }

        // Uses special comparison function for the lower bound and insertion
        // operations.
        void process_site_event() {
            const site_event_type &site_event = *site_events_iterator_;
            site_events_iterator_++;

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
                const site_event_type &site2 = it->first.get_left_site();
                const site_event_type &site3 = it->first.get_right_site();

                // Remove candidate circle from the event queue.
                it->second.deactivate_circle_event();
                it--;
                const site_event_type &site1 = it->first.get_left_site();


                // Insert new arc into the sweepline.
                beach_line_iterator new_node_it = insert_new_arc(site_arc, site_event);

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
        void process_circle_event() { 
            const circle_event_type &circle_event = circle_events_.top();

            // Retrieve the second bisector node that corresponds to the given
            // circle event.
            beach_line_iterator it_first = circle_event.get_bisector();
            beach_line_iterator it_last = it_first;

            // Get the second and the third sites that create given circle event.
            site_event_type site2 = it_first->first.get_left_site();
            site_event_type site3 = it_first->first.get_right_site();

            // Get second bisector;
            edge_type *bisector2 = it_first->second.edge;
            
            // Get first bisector;
            it_first--;
            edge_type *bisector1 = it_first->second.edge;
            
            // Get the first site that creates given circle event.
            site_event_type site1 = it_first->first.get_left_site();

            // Let circle event sites be A, B, C, two bisectors that define
            // circle event be (A, B), (B, C). During circle event processing
            // we remove (A, B), (B, C) and insert (A, C). As beach line nodes 
            // comparer doesn't work fine for the circle events. We only remove
            // (B, C) bisector and change (A, B) bisector to the (A, C). That's
            // why we use const_cast there and take all the responsibility that
            // map data structure keeps correct ordering.
            const_cast<Key &>(it_first->first).set_right_site(site3);
            it_first->second.edge = output_.insert_new_edge(site1, site2, site3, circle_event,
                                                            bisector1, bisector2);
            beach_line_.erase(it_last);
            it_last = it_first;

            // Pop circle event from the event queue, before we might
            // insert new events in it.
            circle_events_.pop();

            // Check the new triplets formed by the neighboring arcs
            // for potential circle events. Check left.
            if (it_first != beach_line_.begin()) {
                it_first->second.deactivate_circle_event();
                it_first--;
                const site_event_type &site_l1 = it_first->first.get_left_site();
                activate_circle_event(site_l1, site1, site3, it_last);
            }

            // Check the new triplets formed by the neighboring arcs
            // for potential circle events. Check right.
            it_last++;
            if (it_last != beach_line_.end()) {
                it_last->second.deactivate_circle_event();
                const site_event_type &site_r1 = it_last->first.get_right_site();
                activate_circle_event(site1, site3, site_r1, it_last);
            }

        }

        // Insert new arc below site arc into the beach line.
        beach_line_iterator insert_new_arc(const site_event_type &site_arc,
                                           const site_event_type &site_event) {
            // Create two new nodes.
            Key new_left_node(site_arc, site_event);
            Key new_right_node(site_event, site_arc);
            
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
            //mpz_class dif_x1, dif_x2, dif_y1, dif_y2, a;
            //dif_x1 = static_cast<int>(site1.x() - site2.x());
            //dif_x2 = static_cast<int>(site2.x() - site3.x());
            //dif_y1 = static_cast<int>(site1.y() - site2.y());
            //dif_y2 = static_cast<int>(site2.y() - site3.y());
            //a = (dif_x1 * dif_y2 - dif_y1 * dif_x2) * 2;
            //
            //// Check if bisectors intersect.
            //if (a >= 0)
            //    return false;

            //mpz_class sum_x1, sum_x2, sum_y1, sum_y2, b1, b2;
            //sum_x1 = static_cast<int>(site1.x() + site2.x());
            //sum_x2 = static_cast<int>(site2.x() + site3.x());
            //sum_y1 = static_cast<int>(site1.y() + site2.y());
            //sum_y2 = static_cast<int>(site2.y() + site3.y());
            //b1 = dif_x1 * sum_x1 + dif_y1 * sum_y1;
            //b2 = dif_x2 * sum_x2 + dif_y2 * sum_y2;

            //mpq_class c_x(b1 * dif_y2 - b2 * dif_y1, a);
            //c_x.canonicalize();
            //mpq_class c_y(b2 * dif_x1 - b1 * dif_x2, a);
            //c_y.canonicalize();
            //mpq_class temp_x(c_x - site1.x());
            //mpq_class temp_y(c_y - site1.y());
            //mpq_class sqr_radius(temp_x * temp_x + temp_y * temp_y);

            //// Create new circle event;
            //c_event = detail::make_circle_event<coordinate_type>(c_x.get_d(),
            //                                                     c_y.get_d(),
            //                                                     sqr_radius.get_d());
            //c_event.set_sites(site1.get_site_index(),
            //                  site2.get_site_index(),
            //                  site3.get_site_index());
            //return true;

            // Check if bisectors intersect.
            if (!detail::left_orientation_test(site1.get_point0(),
                                               site2.get_point0(),
                                               site3.get_point0()))
                return false;

            coordinate_type a = ((site1.x() - site2.x()) * (site2.y() - site3.y()) -
                                 (site1.y() - site2.y()) * (site2.x() - site3.x())) *
                                static_cast<coordinate_type>(2.0);
            
            coordinate_type b1 = (site1.x() - site2.x()) * (site1.x() + site2.x()) +
                                 (site1.y() - site2.y()) * (site1.y() + site2.y());
            coordinate_type b2 = (site2.x() - site3.x()) * (site2.x() + site3.x()) +
                           (site2.y() - site3.y()) * (site2.y() + site3.y());

            // Create new circle event.
            coordinate_type c_x = (b1*(site2.y() - site3.y()) - b2*(site1.y() - site2.y())) / a;
            coordinate_type c_y = (b2*(site1.x() - site2.x()) - b1*(site2.x() - site3.x())) / a;
            coordinate_type sqr_radius = (c_x-site1.x())*(c_x-site1.x()) +
                                         (c_y-site1.y())*(c_y-site1.y());
            c_event = detail::make_circle_event<coordinate_type>(c_x, c_y, sqr_radius);
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
                bisector_node->second.activate_circle_event(circle_events_.push(c_event));
            }
        }

    private:
        std::vector<site_event_type> site_events_;
        site_events_iterator_type site_events_iterator_;
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

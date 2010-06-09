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

    template <typename Point2D>
    struct beach_line_node {
    public:
        typedef typename Point2D Point2D;
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;

        beach_line_node() {}

        explicit beach_line_node(const Point2D &new_point) {
            left_point_ = new_point;
            right_point_ = new_point;
        }

        beach_line_node(const Point2D &left_point, const Point2D &right_point) {
            left_point_ = left_point;
            right_point_ = right_point;
        }

        const Point2D &get_left_point() const {
            return left_point_;
        }

        const Point2D &get_right_point() const {
            return right_point_;
        }

        coordinate_type get_sweepline_coord() const {
            return std::max(left_point_.x(), right_point_.x());
        }

        const Point2D& get_new_point() const {
            if (left_point_.x() > right_point_.x())
                return left_point_;
            return right_point_;
        }

        Point2D get_intersection_point(bool use_right_point, const Point2D &new_point) const {
            const Point2D &use_point = (use_right_point) ? right_point_ : left_point_;
            coordinate_type vertex_x = ((new_point.y() - use_point.y()) *
                                        (new_point.y() - use_point.y()) /
                                        (use_point.x() - new_point.x()) +
                                         use_point.x() + new_point.x()) *
                                         static_cast<coordinate_type>(0.5);
            return make_point_2d(vertex_x, new_point.y());
        }

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

        bool operator() (const BeachLineNode &node1, const BeachLineNode &node2) const {
            coordinate_type node1_line = node1.get_sweepline_coord();
            coordinate_type node2_line = node2.get_sweepline_coord();
            if (node1_line == node2_line) {
                if (node1.get_left_point() == node2.get_right_point() &&
                    node1.get_right_point() == node2.get_left_point())
                    return node1.get_right_point().x() == node1_line;
                return node1.get_left_point().y() <= node2.get_left_point().y();
            }
            else if (node1_line < node2_line)
                return node1.less(node2.get_new_point());
            else
                return !node2.less(node1.get_new_point());
        }
    };

    template <typename Key, typename Value, typename NodeComparer, typename EventQueue>
    class beach_line {
    public:
        typedef typename Key::Point2D Point2D;
        typedef typename Key::coordinate_type coordinate_type;
        typedef typename Key::site_event_type site_event_type;
        typedef typename Key::circle_event_type circle_event_type;
        typedef typename std::map< Key, Value, node_comparer<Key> >::const_iterator beach_line_iterator;
        typedef typename std::pair< beach_line_iterator, bool > beach_line_pair;

        struct event_processor {
            explicit event_processor(beach_line *b_line)
                : beach_line_(b_line) {}

            void operator()(const site_event_type &site_event) {
                beach_line_->process_site_event(site_event);
            }

            void operator()(const circle_event_type &circle_event) {
                beach_line_->process_circle_event(circle_event);
            }

            beach_line* beach_line_;
        };

        beach_line() {
            event_queue_ = new EventQueue;
            event_processor_ = new event_processor(this);
        }

        ~beach_line() {
            if (event_queue_) {
                delete event_queue_;
                event_queue_ = NULL;
            }
            if (event_processor_) {
                delete event_processor_;
                event_processor_ = NULL;
            }
        }

        void init(const std::vector<Point2D> &sites) {
            std::sort(sites.begin(), sites.end());
            int skip = 0;

            if (sites.size() == 1) {
                skip = 1
            } else {
                std::vector<Point2D>::const_iterator it = sites.begin();
                while(it != sites.end() && it->x() == sites.begin()->x()) {
                    it++;
                    skip++;
                }

                if (num == 1) {
                    init_beach_line(*sites.begin(), *it);
                } else {
                    init_beach_line(sites.begin(), it);
                }
            }
            event_queue_->init(sites, skip);
        }

        void reset() {
            event_queue_.reset();
            beach_line_.clear();
        }

        void run_sweepline() {
            while (!event_queue_->empty()) {
                event_queue_->process_top_event(*event_processor_);
                event_queue_->pop();
            }
        }

        void process_site_event(const site_event_type &site_event) {
            Key new_key(site_event.get_point());
            beach_line_iterator it = beach_line_.lower_bound(new_key);

            Point2D point_arc;
            Point2D voronoi_vertex;
            if (it == beach_line_.end()) {
                it--;
                point_arc = it->first.get_right_point();
                voronoi_vertex = it->first.get_intersection_point(true, site_event.get_point());
                activate_circle_event(it->first.get_left_point(),
                                    it->first.get_right_point(),
                                    site_event.get_point());
            } else if (it == beach_line_.begin()) {
                point_arc = it->first.get_left_point();
                voronoi_vertex = it->first.get_intersection_point(false, site_event.get_point());
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
                
                deactivate_circle_event(point1, point2, point3);
                activate_circle_event(point1, point2, site_event.get_point());
                activate_circle_event(site_event.get_point(), point2, point3);
            }

            Key new_left_node(point_arc, site_event.get_point());
            Key new_right_node(site_event.get_point(), point_arc);
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value()));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value()));
        }

        void process_circle_event(const circle_event_type &circle_event) {
            Key new_key(circle_event.get_bisector_left_point(),
                        circle_event.get_bisector_right_point());
            beach_line_iterator it_first = beach_line_.find(new_key);
            beach_line_iterator it_last = it_first;

            if (it_first == beach_line_.end())
                return;


            Point2D point2 = it_first->first.get_left_point();
            Point2D point3 = it_first->first.get_right_point();
            it_first--;
            it_last++;
            Point2D point1 = it_first->first.get_left_point();
            beach_line_.erase(it_first, it_last);
            Key new_node(point1, point3);
            beach_line_pair it_pair = 
                beach_line_.insert(std::pair<Key, Value>(new_node, Value()));

            it_first = it_pair.first;
            it_last = it_first;

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

        void init_beach_line(const Point2D &first_point, const Point2D &second_point) {
            beach_line_node new_left_node(firs_point, second_point);
            beach_line_node new_second_node(second_point, first_point);
            beach_line_.insert(std::pair<Key, Value>(new_left_node, Value()));
            beach_line_.insert(std::pair<Key, Value>(new_right_node, Value()));
        }

        bool create_circle_event(const Point2D &point1,
                                 const Point2D &point2,
                                 const Point2D &point3,
                                 circle_event_type &c_event) const {
            coordinate_type a = (point1.x() - point2.x())*(point2.y() - point3.y()) -
                                (point1.y() - point2.y())*(point2.x() - point3.x());
            if (a <= static_cast<coordinate_type>(0))
                return false;
            coordinate_type b1 = (point1.x() - point2.x())*(point1.x() + point2.x()) +
                                 (point1.y() - point2.y())*(point1.y() + point2.y());
            coordinate_type b2 = (point2.x() - point3.x())*(point2.x() + point3.x()) +
                                 (point2.y() - point3.y())*(point2.y() + point3.y());
            coordinate_type c_x = (b1*(point2.y() - point3.y()) - b2*(point1.y() - point2.y())) / a *
                                  static_cast<coordinate_type>(0.5);
            coordinate_type c_y = (b2*(point1.x() - point2.x()) - b1*(point2.x() - point3.x())) / a *
                                  static_cast<coordinate_type>(0.5);
            coordinate_type sqr_radius = (c_x-point1.x())*(c_x-point1.x()) + (c_y-point1.y())*(c_y-point1.y());
            coordinate_type radius = static_cast<coordinate_type>(sqr_radius);
            c_event = make_circle_event(c_x + radius, c_y);
            c_event.set_bisector(point2, point3);
            return true;
        }

        void activate_circle_event(const Point2D &point1,
                                   const Point2D &point2,
                                   const Point2D &point3) const {
            circle_event_type c_event;
            if (create_circle_event(point1, point2, point3, c_event))
                event_queue_->push(c_event);
        }

        void deactivate_circle_event(const Point2D &point1,
                                     const Point2D &point2,
                                     const Point2D &point3) const {
            circle_event_type c_event;
            if (create_circle_event(point1, point2, point3, c_event))
                event_queue_->deactivate_event(c_event);
        }

    private:
        EventQueue* event_queue_;
        event_processor* event_processor_;
        std::map< Key, Value, NodeComparer > beach_line_;
    };

} // sweepline
} // boost

#endif
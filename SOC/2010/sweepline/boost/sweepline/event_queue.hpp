// Boost sweepline/event_queue.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_EVENT_QUEUE
#define BOOST_SWEPPLINE_EVENT_QUEUE

#include <queue>

namespace boost {
namespace sweepline {

    // Event queue data structure, contains two types of events:
    // site events and circle events.
    template <typename Point2D>
    class event_queue {
    public:
        typedef typename Point2D::coordinate_type coordinate_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;

        enum kEventType {
            SITE_EVENT = 0,
            CIRCLE_EVENT = 1,
            NONE = 2,
        };

        event_queue() {
            site_events_iterator_ = site_events_.begin();
        }

        // Init event queue with sites, starting from the element with
        // skip index. Vector of sites should be sorted.
        void init(const std::vector<Point2D> &sites, int skip) {
            site_events_.clear();
            site_events_.resize(sites.size() - skip);
            std::vector<Point2D>::const_iterator sites_it;
            int index = 0;
            for (sites_it = sites.begin() + skip; sites_it != sites.end(); sites_it++, index++)
                site_events_[index] = make_site_event(sites_it->x(),
                                                      sites_it->y(),
                                                      index + skip);
            init_site_events();
        }

        void reset() {
            site_events_iterator_ = site_events_.begin();
            while (!circle_events_.empty())
                circle_events_.pop();
            while (!deactivated_events_.empty())
                deactivated_events_.pop();
        }

        bool empty() {
            if (site_events_iterator_ != site_events_.end())
                return false;
            remove_not_active_events();
            if (!circle_events_.empty())
                return false;
            return true;
        }

        template <typename event_handler>
        void process_top_event(event_handler &functor) {
            kEventType top_event_type = get_top_event_type();
            if (top_event_type == SITE_EVENT)
                functor(*site_events_iterator_);
            else
                functor(circle_events_.top());
        }

        void pop() {
            kEventType top_event_type = get_top_event_type();
            if (top_event_type == SITE_EVENT)
                site_events_iterator_++;
            else if (top_event_type == CIRCLE_EVENT)
                circle_events_.pop();
        }

        void push(const circle_event_type &circle_event) {
            circle_events_.push(circle_event);
        }

        void deactivate_event(const circle_event_type &circle_event) {
            deactivated_events_.push(circle_event);
        }

    private:
        void init_site_events() {
            site_events_iterator_ = site_events_.begin();
        }

        void remove_not_active_events() {
            while (!circle_events_.empty() && !deactivated_events_.empty() &&
                   circle_events_.top().equals(deactivated_events_.top())) {
                circle_events_.pop();
                deactivated_events_.pop();
            }
        }

        kEventType get_top_event_type() {
            remove_not_active_events();
            if (site_events_iterator_ == site_events_.end())
                return CIRCLE_EVENT;
            else if (circle_events_.empty())
                return SITE_EVENT;
            else {
                // If two event points have the same coordinates return
                // site event at first.
                if (circle_events_.top().compare(*site_events_iterator_) >= 0)
                    return SITE_EVENT;
                else
                    return CIRCLE_EVENT;
            }
            return NONE;
        }

        typename std::vector<site_event_type>::const_iterator 
            site_events_iterator_;
        std::vector<site_event_type> site_events_;
        std::priority_queue< circle_event_type,
                             std::vector<circle_event_type>,
                             std::greater<circle_event_type> > circle_events_;
        std::priority_queue< circle_event_type,
                             std::vector<circle_event_type>,
                             std::greater<circle_event_type> > deactivated_events_;

        //Disallow copy constructor and operator=
        event_queue(const event_queue&);
        void operator=(const event_queue&);
    };

} // sweepline
} // boost

#endif
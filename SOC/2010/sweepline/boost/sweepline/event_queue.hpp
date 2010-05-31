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
        typedef typename Point2D::sweepline_event_type sweepline_event_type;
        typedef typename Point2D::site_event_type site_event_type;
        typedef typename Point2D::circle_event_type circle_event_type;
        typedef typename Point2D::sweepline_event_type::kEventType kEventType;

        event_queue() {
            site_events_iterator_ = site_events_.begin();
        }

        void init(const std::vector<Point2D> &sites) {
            site_events_.clear();
            site_events_.resize(sites.size());
            for (int i = 0; i < sites.size(); i++)
                site_events_[i] = make_site_event(sites[i].x(), sites[i].y());
            init_site_events();
        }

        void reset() {
            site_events_iterator_ = site_events_.begin();
            while (!circle_events_.empty())
                circle_events_.pop();
        }

        bool empty() {
            if (site_events_iterator_ != site_events_.end())
                return false;

            remove_not_active_events();
            if (!circle_events_.empty())
                return false;

            return true;
        }

        const sweepline_event_type &top() {
            kEventType top_event_type = get_top_event_type();
            if (top_event_type == kEventType::SITE_EVENT)
                return *site_events_iterator_;
            else
                return circle_events_.top();
        }

        void pop() {
            kEventType top_event_type = get_top_event_type();
            if (top_event_type == kEventType::SITE_EVENT)
                site_events_iterator_++;
            else if (top_event_type == kEventType::CIRCLE_EVENT)
                circle_events_.pop();
        }

        void push(circle_event_type circle_event) {
            circle_events_.push(circle_event);
        }

    private:
        void init_site_events() {
            std::sort(site_events_.begin(), site_events_.end());
            site_events_iterator_ = site_events_.begin();
            last_event_type_ = kEventType::SITE_EVENT;
        }

        void remove_not_active_events() {
            while (!circle_events_.empty() && 
                   !circle_events_.top().is_active())
                circle_events_.pop();
        }

        kEventType get_top_event_type() {
            remove_not_active_events();

            if (site_events_iterator_ == site_events_.end())
                return kEventType::CIRCLE_EVENT;
            else if (circle_events_.empty())
                return kEventType::SITE_EVENT;
            else {
                if ((*site_events_iterator_) <= circle_events_.top())
                    return kEventType::SITE_EVENT;
                else
                    return kEventType::CIRCLE_EVENT;
            }
            return kEventType::NONE;
        }

        typename std::vector<site_event_type>::const_iterator 
            site_events_iterator_;
        std::vector<site_event_type> site_events_;
        std::priority_queue< circle_event_type,
                             std::vector<circle_event_type>,
                             std::greater<circle_event_type> > circle_events_;
        kEventType last_event_type_;

        //Disallow copy constructor and operator=
        event_queue(const event_queue&);
        void operator=(const event_queue&);
    };

} // sweepline
} // boost

#endif
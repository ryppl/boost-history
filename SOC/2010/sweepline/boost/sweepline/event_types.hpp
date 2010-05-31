// Boost sweepline/event_types.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_EVENT_TYPES
#define BOOST_SWEEPLINE_EVENT_TYPES

namespace boost {
namespace sweepline {
    
    template <typename T>
    class sweepline_event;

    template <typename T>
    class site_event;
    
    template <typename T>
    class circle_event;

    template <typename T>
    class point_2d {
    public:
        typedef T coordinate_type;
        typedef sweepline_event<T> sweepline_event_type;
        typedef site_event<T> site_event_type;
        typedef circle_event<T> circle_event_type;

        point_2d() {}

        point_2d(T x, T y) {
            x_ = x;
            y_ = y;
        }

        bool operator==(const point_2d &point) const {
            return (this->x_ == point.x()) && (this->y_ == point.y());
        }

        bool operator!=(const point_2d &point) const {
            return (this->x_ != point.x) || (this->y_ != point.y());
        }

        bool operator<(const point_2d &point) const {
            if (this->x_ != point.x_)
                return this->x_ < point.x_;
            return this->y_ < point.y_;
        }

        bool operator<=(const point_2d &point) const {
            return !(point < (*this));
        }

        bool operator>(const point_2d &point) const {
            return point < (*this);
        }

        bool operator>=(const point_2d &point) const {
            return !((*this) < point);
        }

        coordinate_type x() const {
            return this->x_;
        }

        coordinate_type y() const {
            return this->y_;
        }

        void x(coordinate_type x) {
            x_ = x;
        }

        void y(coordinate_type y) {
            y_ = y;
        }

    private:
        coordinate_type x_;
        coordinate_type y_;
    };

    template <typename T>
    point_2d<T> make_point_2d(T x, T y) {
        return point_2d<T>(x,y);
    }

    template <typename T>
    class sweepline_event : public point_2d<T> {
    public:
        enum kEventType {
            SITE_EVENT = 0,
            CIRCLE_EVENT = 1,
            NONE = 2,
        };

        sweepline_event() : point_2d() {}

        sweepline_event(T x, T y) : point_2d(x, y) {}

        virtual kEventType get_event_type() const {
            return NONE;
        }
    };

    template <typename T>
    class site_event : public sweepline_event<T> {
    public:
        site_event() : sweepline_event() {}
        
        site_event(T x, T y) : sweepline_event(x, y) {}

        virtual kEventType get_event_type() const {
            return SITE_EVENT;
        }
    };

    template <typename T>
    site_event<T> make_site_event(T x, T y) {
        return site_event<T>(x,y);
    }

    template <typename T>
    class circle_event : public site_event<T> {
    public:
        circle_event() : site_event() { 
            active_ = true;
        }

        circle_event(T x, T y) : site_event(x, y) {
            active_ = true;
        }

        bool is_active() {
            return active_;
        }

        virtual kEventType get_event_type() const {
            return CIRCLE_EVENT;
        }

    private:
        bool active_;
    };

    template <typename T>
    circle_event<T> make_circle_event(T x, T y) {
        return circle_event<T>(x,y);
    }
  
} // sweepline
} // boost

#endif
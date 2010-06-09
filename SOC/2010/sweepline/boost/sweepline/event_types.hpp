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
    struct site_event;
    
    template <typename T>
    struct circle_event;

    template <typename T>
    struct point_2d {
    public:
        typedef T coordinate_type;
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
    struct site_event {
    public:
        typedef T coordinate_type;

        site_event() {}
        
        site_event(T x, T y) : point_(x, y) {}

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

        const point_2d<T> &get_point() const {
            return point_;
        }

    private:
        point_2d<T> point_;
    };

    template <typename T>
    site_event<T> make_site_event(T x, T y) {
        return site_event<T>(x,y);
    }

    struct BeachLineNode;

    template <typename T>
    struct circle_event {
    public:
        typedef T coordinate_type;

        circle_event() {}

        circle_event(T x, T y) : point_(x, y) {}

        bool operator==(const circle_event &c_event) const {
            return point_ == c_event.get_point();
        }

        bool operator!=(const circle_event &c_event) const {
            return point_ != c_event.get_point();
        }

        bool operator<(const circle_event &c_event) const {
            return point_ < c_event.get_point();
        }

        bool operator<=(const circle_event &c_event) const {
            return point_ <= c_event.get_point();
        }

        bool operator>(const circle_event &c_event) const {
            return point_ > c_event.get_point();
        }

        bool operator>=(const circle_event &c_event) const {
            return point_ >= c_event.get_point();
        }

        coordinate_type x() const {
            return point_.x();
        }

        coordinate_type y() const {
            return point_.y();
        }

        const point_2d<T> &get_point() const {
            return point_;
        }

        void set_bisector(const point_2d<T> &left_point, const point_2d<T> &right_point) {
            bisector_left_point_ = left_point;
            bisector_right_point_ = right_point;
        }

        const point_2d<T> &get_bisector_left_point() const {
            return bisector_left_point_;
        }

        const point_2d<T> &get_bisector_right_point() const {
            return bisector_right_point_;
        }

    private:
        point_2d<T> point_;
        point_2d<T> bisector_left_point_;
        point_2d<T> bisector_right_point_;
    };

    template <typename T>
    circle_event<T> make_circle_event(T x, T y) {
        return circle_event<T>(x,y);
    }
  
} // sweepline
} // boost

#endif
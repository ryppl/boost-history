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

    // Point in 2D space data structure. Comparators defined in this
    // datascturcture actually define sweepline movement direction.
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
            return (this->x_ != point.x()) || (this->y_ != point.y());
        }

        // This comparator actually defines sweepline movement direction.
        // Sweepline will move in the horizontal direction:
        // from left to right or from right to left.
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

    // Site event type. Occurs when sweepline sweeps over one of the initial sites.
    template <typename T>
    struct site_event {
    public:
        typedef T coordinate_type;

        site_event() {}
        
        site_event(T x, T y, int index) : point_(x, y), site_index_(index) {}

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

        int get_site_index() const {
            return site_index_;
        }

    private:
        point_2d<T> point_;
        int site_index_;
    };

    template <typename T>
    site_event<T> make_site_event(T x, T y, int index) {
        return site_event<T>(x, y, index);
    }

    // Circle event type. Occurs when sweepline sweeps over the bottom point of
    // the voronoi circle (with center at the bisectors intersection point).
    // Circle event contains circle center and squared radius (to avoid sqrt
    // computation). To compare bottom points of two different voronoi circles
    // we don't compute exact radius and use special arithmetic for that. This 
    // way voronoi diagram implementation may be used with rational arithmetic.
    // Let circle center coordinates be (x, y), squared radius be r. 
    // Bottom point of the voronoi circle will be defined as (x + sqrt(r), y).
    template <typename T>
    struct circle_event {
    public:
        typedef T coordinate_type;

        circle_event() {}

        circle_event(T c_x, T c_y, T sqr_r) :
        center_(c_x, c_y), sqr_radius_(sqr_r) {}

        bool equals(const circle_event &c_event) const {
            return center_.x() == c_event.x() && center_.y() == c_event.y() &&
                   sqr_radius_ == c_event.get_sqr_radius();
        }

        bool operator==(const circle_event &c_event) const {
            if (center_.y() != c_event.y())
                return false;

            T sqr_dif_x = (center_.x() - c_event.x()) *
                          (center_.x() - c_event.x());
            T sum_r_sqr = sqr_radius_ + c_event.get_sqr_radius();
            T value_left = (sum_r_sqr - sqr_dif_x) * (sum_r_sqr - sqr_dif_x);
            T value_right = static_cast<T>(4) * sqr_radius_ *
                            c_event.get_sqr_radius();

            return value_left == value_right;
        }

        bool operator!=(const circle_event &c_event) const {
            return !((*this) == c_event);
        }

        bool operator<(const circle_event &c_event) const {
            T x1 = center_.x();
            T y1 = center_.y();
            T sqr_r1 = sqr_radius_;
            T x2 = c_event.x();
            T y2 = c_event.y();
            T sqr_r2 = c_event.get_sqr_radius();

            T sqr_dif_x = (x1 - x2) * (x1 - x2);
            T sum_r_sqr = sqr_r1 + sqr_r2;
            T value_left = (sum_r_sqr - sqr_dif_x) * (sum_r_sqr - sqr_dif_x);
            T value_right = static_cast<T>(4) * sqr_r1 * sqr_r2;
            
            if (x1 > x2) {
                if (sqr_r2 <= sqr_r1)
                    return false;
                
                if (sqr_dif_x >= sum_r_sqr)
                    return false;

                if (value_left == value_right)
                    return y1 < y2;
                else
                    return value_left > value_right;
            }
            else if (x1 < x2) {
                if (sqr_r1 <= sqr_r2)
                    return true;

                if (sqr_dif_x >= sum_r_sqr)
                    return true;

                if (value_left == value_right)
                    return y1 < y2;
                else
                    return value_left < value_right;
            }
            else {
                if (sqr_r1 != sqr_r2)
                    return sqr_r1 < sqr_r2;
                return y1 < y2;
            }
        }

        bool operator<=(const circle_event &c_event) const {
            return !(c_event < (*this));
        }

        bool operator>(const circle_event &c_event) const {
            return c_event < (*this);
        }

        bool operator>=(const circle_event &c_event) const {
            return !((*this) < c_event);
        }

        // Compares bottom voronoi circle point with site event point.
        // If circle point is less then site point return -1.
        // If circle point is equal to site point return 0.
        // If circle point is greater then site point return 1.
        int compare(const site_event<T> &s_event) const {
            if (s_event.x() < center_.x())
                return 1;
            T sqr_dif_x = (s_event.x() - center_.x()) *
                          (s_event.x() - center_.x());
            if (sqr_dif_x == sqr_radius_) {
                if (center_.y() == s_event.y())
                    return 0;
                return (center_.y() < s_event.y()) ? -1 : 1;
            }
            return (sqr_dif_x < sqr_radius_) ? 1 : -1;
        }

        coordinate_type x() const {
            return center_.x();
        }

        coordinate_type y() const {
            return center_.y();
        }

        const point_2d<T> &get_center() const {
            return center_;
        }

        const T &get_sqr_radius() const {
            return sqr_radius_;
        }

        void set_bisector(const site_event<T> &left_site,
                          const site_event<T> &right_site) {
            bisector_left_site_ = left_site;
            bisector_right_site_ = right_site;
        }

        const site_event<T> &get_bisector_left_site() const {
            return bisector_left_site_;
        }

        const site_event<T> &get_bisector_right_site() const {
            return bisector_right_site_;
        }

    private:
        point_2d<T> center_;
        T sqr_radius_;
        site_event<T> bisector_left_site_;
        site_event<T> bisector_right_site_;
    };

    template <typename T>
    circle_event<T> make_circle_event(T center_x, T center_y, T sqr_radius) {
        return circle_event<T>(center_x, center_y, sqr_radius);
    }
  
} // sweepline
} // boost

#endif
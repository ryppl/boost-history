//  boost/stopwatches/t24_hours.hpp  -----------------------------------------------------------//
//  Copyright 2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org/libs/stopwatches for documentation.

#ifndef BOOST_STOPWATCHES_T24_HOURS_HPP
#define BOOST_STOPWATCHES_T24_HOURS_HPP

#include <boost/chrono/chrono.hpp>


namespace boost { namespace stopwatches {

class t24_hours {
    typedef boost::chrono::duration<boost::int_least32_t, ratio<24*3600> > days;
    typedef boost::chrono::hours hours;
    typedef boost::chrono::minutes minutes;
    typedef boost::chrono::seconds seconds;
    typedef boost::chrono::nanoseconds nanoseconds;
public:
    days days_;
    hours hours_;
    minutes minutes_;
    seconds seconds_;
    nanoseconds nanoseconds_;

    template <class Rep, class Period>
    static days get_days(const boost::chrono::duration<Rep, Period>& d) {
        return boost::chrono::duration_cast<days>(d);
    }

    template <class Rep, class Period>
    static hours get_hours(const boost::chrono::duration<Rep, Period>& d) {
        return boost::chrono::duration_cast<hours>(d % days(1));
    }

    template <class Rep, class Period>
    static minutes get_minutes(const boost::chrono::duration<Rep, Period>& d) {
        return boost::chrono::duration_cast<minutes>(d % hours(1));
    }

    template <class Rep, class Period>
    static seconds get_seconds(const boost::chrono::duration<Rep, Period>& d) {
        return boost::chrono::duration_cast<seconds>(d % minutes(1));
    }

    template <class Rep, class Period>
    static nanoseconds get_nanoseconds(const boost::chrono::duration<Rep, Period>& d) {
        return boost::chrono::duration_cast<nanoseconds>(d % seconds(1));
    }

    template <class Rep, class Period>
        explicit t24_hours(const boost::chrono::duration<Rep, Period>& d)
        : days_  (get_days(d))
        , hours_  (get_hours(d))
        , minutes_ (get_minutes(d))
        , seconds_ (get_seconds(d))
        , nanoseconds_ (get_nanoseconds(d))
    {}
};

} // namespace stopwatches
} // namespace boost


#endif  // BOOST_STOPWATCHES_PROCESS_CLOCK_HPP

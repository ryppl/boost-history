//  (C) Copyright Beman Dawes 1994-99. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

#ifndef NUMERICS_TIMER_H
#define NUMERICS_TIMER_H

#ifndef USE_GCC
#include <limits>
#endif
#include <ctime>

// This is a cover of the boost timer.
namespace numerics {

#ifdef USE_MSVC

    class timer {
    private:
        ::clock_t start_time_; 

    public:
        timer (): start_time_ (::clock ()) {}
        timer (const timer &t): start_time_ (t.start_time_) {}
        ~timer () {}
        timer &operator = (const timer &t) {
            start_time_ = t.start_time_;
            return *this;
        }
        void restart () {
            start_time_ = ::clock ();
        }
        double elapsed () const {
            return double (::clock () - start_time_) / CLOCKS_PER_SEC; 
        }                

        double elapsed_max () const {
            return (double (std::numeric_limits<::clock_t>::max ()) -
                    double (start_time_)) / double (CLOCKS_PER_SEC); 
        }
        double elapsed_min () const {
            return double (1) / double (CLOCKS_PER_SEC); 
        }
    }; 

#else // USE_MSVC

    class timer {
    private:
        std::clock_t start_time_; 

    public:
        timer (): start_time_ (std::clock ()) {}
        timer (const timer &t): start_time_ (t.start_time_) {}
        ~timer () {}
        timer &operator = (const timer &t) {
            start_time_ = t.start_time_;
            return *this;
        }
        void restart () {
            start_time_ = std::clock ();
        }
        double elapsed () const {
            return double (std::clock () - start_time_) / CLOCKS_PER_SEC; 
        }                

#ifndef USE_GCC
        double elapsed_max () const {
            return (double (std::numeric_limits<std::clock_t>::max ()) -
                    double (start_time_)) / double (CLOCKS_PER_SEC); 
        }
        double elapsed_min () const {
            return double (1) / double (CLOCKS_PER_SEC); 
        }
#endif // USE_GCC
    }; 

#endif // USE_MSVC

}

#endif // NUMERICS_TIMER_H


//  boost/chrono/suspendible_clock.hpp  -----------------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org/libs/stopwatches for documentation.

#ifndef BOOST_STOPWATCHES_SUSPENDIBLE_CLOCK_HPP
#define BOOST_STOPWATCHES_SUSPENDIBLE_CLOCK_HPP

#include <boost/version.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/stopwatches/clock_suspender.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread/tss.hpp>
#include <memory>

namespace boost { namespace stopwatches {

    template < class Clock=chrono::high_resolution_clock >
    class suspendible_clock {
    public:
        typedef typename Clock::duration                           duration;
        typedef typename Clock::rep                                       rep;
        typedef typename Clock::period                                    period;
        typedef chrono::time_point<suspendible_clock<Clock> >  time_point;
        static const bool is_steady =             true;

    private:
        struct thread_specific_context {
            bool suspended_;
            time_point suspended_time_;
            duration suspended_duration_;
            std::size_t suspend_level_;

            thread_specific_context() 
                : suspended_(false)
                , suspended_time_()
                , suspended_duration_(duration::zero())
                , suspend_level_(0)
            {}
            duration suspended(system::error_code & ec = BOOST_CHRONO_THROWS) {
                if (!suspended_) {
                    ec.clear();
                    return suspended_duration_;                   
                } else {
                    time_point tmp;
                    tmp+=duration(Clock::now(ec).time_since_epoch());
		    if (!BOOST_CHRONO_IS_THROWS(ec)) {
                      if (ec) return duration::zero();
		    }
                    return suspended_duration_ + tmp - suspended_time_;
                }
            }

            void suspend( system::error_code & ec = BOOST_CHRONO_THROWS ) {
                if (!suspended_) {
                    time_point tmp;
                    tmp+=duration(Clock::now(ec).time_since_epoch());
		    if (!BOOST_CHRONO_IS_THROWS(ec)) {
                      if (ec) return;
		    }
                    ++suspend_level_;
                    suspended_time_ = tmp;
                    suspended_=true;
                } else {
                    ec.clear();
                    ++suspend_level_;
                }
            }
            void resume( system::error_code & ec = BOOST_CHRONO_THROWS ) {
                if (suspended_&&(--suspend_level_==0)) {
                    time_point tmp;
                    tmp+=duration(Clock::now(ec).time_since_epoch());
		    if (!BOOST_CHRONO_IS_THROWS(ec)) {
                      if (ec) return;
		    }
                    suspended_duration_ += tmp - suspended_time_;
                    suspended_=false;
                } else {
                    ec.clear();
                }                    
            }

        };
        static thread_specific_context* instance(system::error_code & ec) {
            thread_specific_context* ptr= ptr_.get();
            if (ptr==0) {
                if (BOOST_CHRONO_IS_THROWS(ec))  {
                    std::auto_ptr<thread_specific_context> ptr2(new thread_specific_context());
                    ptr_.reset(ptr2.get());
                    ptr = ptr2.release();
                } else {
                    ptr=(new(std::nothrow) thread_specific_context());
                    if (ptr==0) {
#if (BOOST_VERSION / 100 % 1000) < 44
                        ec.assign( system::errc::resource_unavailable_try_again, system::generic_category );
#else
                        ec.assign( system::errc::resource_unavailable_try_again, system::generic_category() );
#endif          
                        return 0;
                    }
                    try {
                        ptr_.reset(ptr);
                    } catch (...) {
#if (BOOST_VERSION / 100 % 1000) < 44
                        ec.assign( system::errc::resource_unavailable_try_again, system::generic_category );
#else
                        ec.assign( system::errc::resource_unavailable_try_again, system::generic_category() );
#endif          
                        return 0;
                    }
                }
            }
            ec.clear();
            return ptr;
        }

        static thread_specific_ptr<thread_specific_context> ptr_;
    public:
        static time_point now( ) {
            time_point res;
            res+= duration(Clock::now().time_since_epoch())-suspended();
            return res;
        }

        static time_point now( system::error_code & ec ) {
            time_point res;
            typename Clock::time_point t=Clock::now(ec);
	    if (!BOOST_CHRONO_IS_THROWS(ec)) {
              if (ec) return time_point();
	    }
            res+= duration(t.time_since_epoch())-suspended(ec);
	    if (!BOOST_CHRONO_IS_THROWS(ec)) {
              if (ec) return time_point();
	    }
            return res;
        }

        static void suspend( system::error_code & ec = BOOST_CHRONO_THROWS ) {
            thread_specific_context* ptr= instance(ec);
            if (ptr!=0) ptr->suspend(ec);
        }
        static void resume( system::error_code & ec = BOOST_CHRONO_THROWS ) {
            thread_specific_context* ptr= instance(ec);
            if (ptr!=0) ptr->resume(ec);
        }
        static duration suspended(system::error_code & ec = BOOST_CHRONO_THROWS)
        {
            thread_specific_context* ptr= instance(ec);
            if (ptr!=0) {
                return ptr->suspended(ec);
            }
            else return duration::zero();
        }
        class clock_suspender {
        public:
            clock_suspender(system::error_code & ec = BOOST_CHRONO_THROWS)
                : ptr_(instance(ec))
            {
                if (ptr_!=0) ptr_->suspend(ec);
            }
            ~clock_suspender() {
                if (ptr_!=0) {
                    system::error_code ec;
                    ptr_->resume(ec);
                }
                
            }
        private:
            thread_specific_context* ptr_;
            //~ clock_suspender(); // = delete;
            clock_suspender(const clock_suspender&); // = delete;
            clock_suspender& operator=(const clock_suspender&); // = delete;
        };

    };

    template < class Clock >
    thread_specific_ptr<typename suspendible_clock<Clock>::thread_specific_context> suspendible_clock<Clock>::ptr_;

    template <class Clock>
    struct is_suspendible<suspendible_clock<Clock> > : mpl:: true_ {};

    template <class Clock>
    class clock_suspender<suspendible_clock<Clock> >
        : public suspendible_clock<Clock>::clock_suspender {
    public:
        clock_suspender(system::error_code & ec = BOOST_CHRONO_THROWS) : suspendible_clock<Clock>::clock_suspender(ec) {}
    private:
        //~ clock_suspender(); // = delete;
        clock_suspender(const clock_suspender&); // = delete;
        clock_suspender& operator=(const clock_suspender&); // = delete;
    };

} // namespace stopwatches
} // namespace boost

#endif  // BOOST_CHRONO_SUSPENDIBLE_CLOCK_HPP

//  boost/chrono/process_cpu_clocks.hpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_SUSPENDABLE_CLOCK_HPP
#define BOOST_CHRONO_SUSPENDABLE_CLOCK_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/scoped_suspend.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread/tss.hpp>
#include <memory>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace chrono {

    template < class Clock >
    class suspendable_clock {
    public:        
        typedef typename Clock::duration                           duration;
        typedef typename Clock::rep                                       rep;
        typedef typename Clock::period                                    period;
        typedef chrono::time_point<suspendable_clock<Clock> >  time_point;
        static const bool is_monotonic =             true;

    private:
        struct thread_specific_context {
            bool suspended_;
            time_point suspended_time_;
            duration suspended_duration_;
            std::size_t suspend_level_;
            
            duration suspended(system::error_code & ec = system::throws) {
                if (!suspended_) return suspended_duration_;
                else {
                    time_point tmp;
                    tmp+=duration(Clock::now(ec).time_since_epoch().count());
                    if (ec) return duration::zero();
                    return suspended_duration_ + tmp - suspended_time_;
                }
            }
            
            void suspend( system::error_code & ec = system::throws ) {
                if (!suspended_) {
                    time_point tmp;
                    tmp+=duration(Clock::now(ec).time_since_epoch().count());
                    if (ec) return;
                    ++suspend_level_;
                    suspended_time_ = tmp;
                    suspended_=true;
                } else {
                    ++suspend_level_;
                }
            }
            void resume( system::error_code & ec = system::throws ) {
                if (suspended_&&(--suspend_level_==0)) {
                    time_point tmp;
                    tmp+=duration(Clock::now(ec).time_since_epoch().count());
                    if (ec) return;
                    suspended_duration_ += tmp - suspended_time_;
                    suspended_=false;
                }
            }
            
        };
        static thread_specific_context* instance(system::error_code & ec) {
            thread_specific_context* ptr= ptr_.get();
            if (ptr==0) {
                if (ec == system::throws) {
                    std::auto_ptr<thread_specific_context> ptr2(new thread_specific_context());
                    ptr_.reset(ptr2.get());
                    ptr = ptr2.release();
                } else {
                    ptr=(new(std::nothrow) thread_specific_context());
                    if (ptr==0) {
                        //ec=...
                        return 0;
                    }
                    try {
                        ptr_.reset(ptr);
                    } catch (...) {
                        //ec=...
                        return 0;                        
                    }
                }
            }
            return ptr;
        }

        static thread_specific_ptr<thread_specific_context> ptr_;
    public:
        static time_point now( system::error_code & ec = system::throws ) {
            time_point res;
            res+= duration(Clock::now(ec).time_since_epoch().count())-suspended(ec);
            return res;
        }

        static void suspend( system::error_code & ec = system::throws ) {
            thread_specific_context* ptr= instance(ec);
            if (ptr!=0) ptr->suspend(ec);
        }
        static void resume( system::error_code & ec = system::throws ) {
            thread_specific_context* ptr= instance(ec);
            if (ptr!=0) ptr->resume(ec);
        }
        static duration suspended(system::error_code & ec = system::throws)
        {
            thread_specific_context* ptr= instance(ec);
            if (ptr!=0) {
                return ptr->suspended(ec);
            }
            else return duration::zero();
        }
        class scoped_suspend {
        public:
            scoped_suspend(system::error_code & ec = system::throws)
                : ptr_(instance(ec)) 
            {
                ptr_->suspend(ec);
            }
            ~scoped_suspend() {
                system::error_code ec;
                ptr_->resume(ec);
            }
        private:
            thread_specific_context* ptr_;
            scoped_suspend(const scoped_suspend&); // = delete;
            scoped_suspend& operator=(const scoped_suspend&); // = delete;
        };
    
    };

    template < class Clock >
    thread_specific_ptr<typename suspendable_clock<Clock>::thread_specific_context> suspendable_clock<Clock>::ptr_;
    
    
        
    template <class Clock>
    struct is_suspendable<suspendable_clock<Clock> > : mpl:: true_ {};
        
        

    template <class Clock>
    class scoped_suspend<suspendable_clock<Clock> > 
        : public suspendable_clock<Clock>::scoped_suspend {
    public:
        scoped_suspend(system::error_code & ec = system::throws) : Clock::scoped_suspend(ec) {}
    };        
    
} // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_CHRONO_PROCESS_CPU_CLOCKS_HPP

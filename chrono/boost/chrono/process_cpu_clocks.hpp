//  boost process_cpu_clocks.hpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_PROCESS_CPU_CLOCKS_HPP
#define BOOST_CHRONO_PROCESS_CPU_CLOCKS_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/operators.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace chrono {

    class BOOST_CHRONO_DECL process_real_cpu_clock {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_real_cpu_clock>    time_point;
        static const bool is_monotonic =             true;

        static time_point now( system::error_code & ec = system::throws );
    };

    class BOOST_CHRONO_DECL process_user_cpu_clock {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_user_cpu_clock>    time_point;
        static const bool is_monotonic =             true;

        static time_point now( system::error_code & ec = system::throws );
    };

    class BOOST_CHRONO_DECL process_system_cpu_clock {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_system_cpu_clock>    time_point;
        static const bool is_monotonic =             true;

        static time_point now( system::error_code & ec = system::throws );
    };

    class BOOST_CHRONO_DECL process_cpu_clock
    {
    public:
        struct times : arithmetic<times>, less_than_comparable<times>
        {
            times() 
                : real(0)
                , user(0)
                , system(0){}
            times(
                process_real_cpu_clock::rep r,
                process_user_cpu_clock::rep   u,
                process_system_cpu_clock::rep s)
                : real(r)
                , user(u)
                , system(s){}
            
            process_real_cpu_clock::rep   real;    // real (i.e wall clock) time
            process_user_cpu_clock::rep   user;    // user cpu time
            process_system_cpu_clock::rep system;  // system cpu time

            bool operator==(times const& rhs) {
                return (real==rhs.real &&
                        user==rhs.user &&
                        system==rhs.system);
            }
                    
            times operator+=(times const& rhs) {
                real+=rhs.real;
                user+=rhs.user;
                system+=rhs.system;
                return *this;
            }
            times operator-=(times const& rhs) {
                real-=rhs.real;
                user-=rhs.user;
                system-=rhs.system;
                return *this;
            }
            times operator*=(times const& rhs) {
                real*=rhs.real;
                user*=rhs.user;
                system*=rhs.system;
                return *this;
            }
            times operator/=(times const& rhs) {
                real/=rhs.real;
                user/=rhs.user;
                system/=rhs.system;
                return *this;
            }
            bool operator<(times const & rhs) const { 
                if (real < rhs.real) return true;
                if (real > rhs.real) return false;
                if (user < rhs.user) return true;
                if (user > rhs.user) return false;
                if (user < rhs.user) return true;
                else return false;
            }
        };

        typedef duration<times,  nano>                duration;
        typedef duration::rep                       rep;
        typedef duration::period                    period;
        typedef chrono::time_point<process_cpu_clock>  time_point;
        static const bool is_monotonic =           true;

        static time_point now( system::error_code & ec = system::throws );
    };

    template <>
    struct duration_values<process_cpu_clock::times>
    {
        typedef process_cpu_clock::times Rep;
  public:
        static Rep zero() {return Rep();}
        static Rep max BOOST_PREVENT_MACRO_SUBSTITUTION ()  {
          return Rep((std::numeric_limits<process_real_cpu_clock::rep>::max)(),
                      (std::numeric_limits<process_user_cpu_clock::rep>::max)(),
                      (std::numeric_limits<process_system_cpu_clock::rep>::max)());
        }
        static Rep min BOOST_PREVENT_MACRO_SUBSTITUTION ()  {
          return Rep((std::numeric_limits<process_real_cpu_clock::rep>::min)(),
                      (std::numeric_limits<process_user_cpu_clock::rep>::min)(),
                      (std::numeric_limits<process_system_cpu_clock::rep>::min)());
        }
  };
    
} // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_PROCESS_TIMES_HPP

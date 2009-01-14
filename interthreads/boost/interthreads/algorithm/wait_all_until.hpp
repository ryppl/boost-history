//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_WAIT_ALL_UNTIL__HPP
#define BOOST_INTERTHREADS_WAIT_ALL_UNTIL__HPP

#include <boost/fusion/include/transform.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace fct {
        struct wait_until {
            wait_until(const system_time& abs_time) : abs_time_(abs_time) {}
            template<typename ACT>
            bool operator()(ACT& act) const {
                return act.wait_until(abs_time_);
            }
        private:
            const system_time& abs_time_;
        };
    }

    namespace fct {
        struct wait_for {
            template <typename Duration>
            wait_for(const Duration& rel_time) : abs_time_(get_system_time()+rel_time) {}
            template<typename ACT>
            bool operator()(ACT& act) const {
                return act.wait_until(abs_time_);
            }
        private:
            const system_time& abs_time_;
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct wait_all_until {
            typedef bool type;
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct wait_all_for {
            typedef bool type;
        };
    }
    
    template <typename Sequence>
    typename result_of::wait_all_until<Sequence const> 
    wait_all_until(Sequence const& t, const system_time& abs_time) {
        bool r = fct::wait_until(abs_time)(fusion::at_c<0>(t));
       	if (r) return true;
        else {
            if (fusion::size(t)==1) {
                return false;
            } else {
                return get_all_until(fusion::pop_front(t));
            }
        }
    }

    template <typename Sequence, typename Duration>
    typename result_of::wait_all_for<Sequence> 
    wait_all_for(Sequence& t, const Duration& rel_time) {
        return wait_all_until(t, get_system_time()+rel_time);
    }
    

}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

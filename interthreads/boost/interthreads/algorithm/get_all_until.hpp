//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_GET_ALL_UNTIL__HPP
#define BOOST_INTERTHREADS_GET_ALL_UNTIL__HPP

#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <utility>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif
#include <boost/interthreads/algorithm/get_all.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace fct {
        struct get_until {
            get_until(const system_time& abs_time) : abs_time_(abs_time) {}
            template<typename ACT>
            typename std::pair<bool,typename ACT::result_type> operator()(ACT& act) const {
                return act.get_until(abs_time_);
            }
        private:
            const system_time& abs_time_;
        };
    }

    namespace fct {
        struct get_for {
            template <typename Duration>
            get_for(const Duration& abs_time) : abs_time_(get_system_time()+abs_time) {}
            template<typename ACT>
            typename std::pair<bool,typename ACT::result_type> operator()(ACT& act) const {
                return act.get_until(abs_time_);
            }
        private:
            const system_time& abs_time_;
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct get_all_until {
            typedef std::pair<bool, typename result_of::get_all<Sequence>::type >  type;
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct get_all_for {
            typedef std::pair<bool, typename result_of::get_all<Sequence>::type >  type;
        };
    }
    
    template <typename Sequence>
    typename result_of::get_all_until<Sequence> 
    get_all_until(Sequence& t, const system_time& abs_time) {
        std::pair<bool,typename fusion::result_of::at_c<Sequence, 0>::type::result_type > r = fct::get_until(abs_time)(fusion::at_c<0>(t));
       	if (r.first) return std::make_pair(true, result_of::get_all<Sequence>::type());
        else {
            if (fusion::size(t)==1) {
                return std::make_pair(false, result_of::get_all<Sequence>::type(r.first));
            } else {
                typename result_of::get_all_until<typename fusion::result_of::pop_front<Sequence>::type > tmp= get_all_until(fusion::pop_front(t));
                return std::make_pair(tmp.first, fusion::push_front(tmp.second, r.second));
            }
        }
    }

    template <typename Sequence, typename Duration>
    typename result_of::get_all_for<Sequence> 
    get_all_for(Sequence& t, const Duration& rel_time) {
        return get_all_until(t, get_system_time()+rel_time);
    }


}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

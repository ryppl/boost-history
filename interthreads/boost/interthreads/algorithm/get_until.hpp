//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_GET_UNTIL__HPP
#define BOOST_INTERTHREADS_GET_UNTIL__HPP

#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <utility>
#include <boost/utility/enable_if.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif
#include <boost/interthreads/algorithm/wait_until.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace result_of {
        template <typename ACT>
        struct get_until {
            typedef typename act_traits<ACT>::move_dest_type type;
        };
    }

    namespace result_of {
        template <typename ACT, typename Duration>
        struct get_for {
            typedef typename act_traits<ACT>::move_dest_type type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename ACT >
        struct get_until {
            static typename result_of::template get_until<ACT>::type apply( ACT& act, const system_time& abs_time ) {
                interthreads::wait_until(act, abs_time);
                return act.get();
            }
        };
        template< typename ACT, typename Duration>
        struct get_for {
            static typename result_of::template get_for<ACT,Duration>::type apply( ACT& act, Duration rel_time ) {
                interthreads::wait_for(act, rel_time);
                return act.get();
            }
        };
    }


    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template get_until<ACT>::type
    >::type get_until(ACT& act, const system_time& abs_time) {
        return partial_specialization_workaround::get_until<ACT>::apply(act, abs_time);
    }

    template <typename ACT, typename Duration>
    typename boost::enable_if<has_future_if<ACT>,typename result_of::template get_for<ACT,Duration>::type >::type
    get_for(ACT& act, Duration rel_time) {
        return partial_specialization_workaround::get_for<ACT, Duration>::apply(act, rel_time);
    }


}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

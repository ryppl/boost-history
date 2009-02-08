//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_WAIT_UNTIL__HPP
#define BOOST_INTERTHREADS_WAIT_UNTIL__HPP

#include <boost/fusion/include/transform.hpp>
#include <boost/utility/enable_if.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace result_of {
        template <typename ACT>
        struct wait_until {
            typedef bool type;
        };
    }

    namespace result_of {
        template <typename ACT, typename Duration>
        struct wait_for {
            typedef bool type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename ACT >
        struct wait_until {
            static typename result_of::template wait_until<ACT>::type apply( ACT& act, const system_time& abs_time ) {
                return act.wait_until(abs_time);
            }
        };
        template< typename ACT >
        struct wait_until<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template wait_until<ACT>::type apply( boost::detail::thread_move_t<ACT> act, const system_time& abs_time ) {
                return act.wait_until(abs_time);
            }
        };
        template< typename ACT, typename Duration>
        struct wait_for {
            static typename result_of::template wait_for<ACT,Duration>::type apply( ACT& act, Duration abs_time ) {
                return act.wait_for(abs_time);
            }
        };
        template< typename ACT, typename Duration>
        struct wait_for<boost::detail::thread_move_t<ACT>,Duration> {
            static typename result_of::template wait_for<ACT,Duration>::type apply( boost::detail::thread_move_t<ACT> act, Duration abs_time ) {
                return act.wait_for(abs_time);
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template wait_until<ACT>::type
    >::type
    wait_until(ACT& act, const system_time& abs_time) {
        return partial_specialization_workaround::wait_until<ACT>::apply(act, abs_time);
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template wait_until<ACT>::type
    >::type
    wait_until(boost::detail::thread_move_t<ACT> act, const system_time& abs_time) {
        return partial_specialization_workaround::wait_until<boost::detail::thread_move_t<ACT> >::apply(act, abs_time);
    }

    template <typename ACT, typename Duration>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::wait_for<ACT,Duration>::type
    >::type
    wait_for(ACT& act, Duration rel_time) {
        return partial_specialization_workaround::wait_for<ACT,Duration>::apply(act, rel_time);
    }

    template <typename ACT, typename Duration>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::wait_for<ACT,Duration>::type
    >::type
    wait_for(boost::detail::thread_move_t<ACT> act, Duration rel_time) {
        return partial_specialization_workaround::wait_for<boost::detail::thread_move_t<ACT>,Duration>::apply(act, rel_time);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

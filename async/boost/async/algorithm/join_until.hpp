//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_JOIN_UNTIL__HPP
#define BOOST_ASYNC_JOIN_UNTIL__HPP

#include <boost/fusion/include/transform.hpp>
#include <boost/utility/enable_if.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chrono/chrono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace result_of {
        template <typename ACT>
        struct join_until {
            typedef bool type;
        };
    }
    namespace result_of {
        template <typename ACT, typename Duration>
        struct join_for {
            typedef bool type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename ACT >
        struct join_until {
            static typename result_of::template join_until<ACT>::type apply( ACT& act, const system_time& abs_time ) {
                return act.join_until(abs_time);
            }
        };

        template< typename ACT >
        struct join_until<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template join_until<ACT>::type apply( boost::detail::thread_move_t<ACT> act, const system_time& abs_time ) {
                return act->join_until(abs_time);
            }
        };

        template< typename ACT, typename Duration>
        struct join_for {
            static typename result_of::template join_for<ACT,Duration>::type apply( ACT& act, Duration abs_time ) {
                return act.join_for(abs_time);
            }
        };

        template< typename ACT, typename Duration>
        struct join_for<boost::detail::thread_move_t<ACT>,Duration > {
            static typename result_of::template join_for<ACT,Duration>::type apply( boost::detail::thread_move_t<ACT> act, Duration abs_time ) {
                return act->join_for(abs_time);
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template join_until<ACT>::type
    >::type
    join_until(ACT& act, const system_time& abs_time) {
        return partial_specialization_workaround::join_until<ACT>::apply(act, abs_time);
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template join_until<ACT>::type
    >::type
    join_until(boost::detail::thread_move_t<ACT> act, const system_time& abs_time) {
        return partial_specialization_workaround::join_until<boost::detail::thread_move_t<ACT> >::apply(act, abs_time);
    }

    template <typename ACT, typename Duration>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template join_for<ACT,Duration>::type
    >::type
    join_for(ACT& act, const Duration& rel_time) {
        return partial_specialization_workaround::join_for<ACT,Duration>::apply(act, rel_time);
    }

    template <typename ACT, typename Duration>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template join_for<ACT,Duration>::type
    >::type
    join_for(boost::detail::thread_move_t<ACT> act, const Duration& rel_time) {
        return partial_specialization_workaround::join_for<boost::detail::thread_move_t<ACT>,Duration>::apply(act, rel_time);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_WAIT__HPP
#define BOOST_INTERTHREADS_WAIT__HPP

#include <boost/fusion/include/for_each.hpp>
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
        struct wait {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename ACT >
        struct wait {
            static typename result_of::template wait<ACT>::type apply( ACT& act ) {
                return act.wait();
            }
        };
        template< typename ACT >
        struct wait<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template wait<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                return act->wait();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template wait<ACT>::type
    >::type
    wait(ACT& act) {
        return partial_specialization_workaround::wait<ACT>::apply(act);
    }
    template <typename ACT>
    void wait_act(ACT& act) {
        return partial_specialization_workaround::wait<ACT>::apply(act);
    }
    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template wait<ACT>::type
    >::type
    wait(boost::detail::thread_move_t<ACT> act) {
        return partial_specialization_workaround::wait<boost::detail::thread_move_t<ACT> >::apply(act);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

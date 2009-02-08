//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_JOIN__HPP
#define BOOST_INTERTHREADS_JOIN__HPP

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
        struct join {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename ACT >
        struct join {
            static typename result_of::template join<ACT>::type apply( ACT& act ) {
                return act.join();
            }
        };
        template< typename ACT >
        struct join<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template join<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                return act->join();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template join<ACT>::type
    >::type
    join(ACT& act) {
        return partial_specialization_workaround::join<ACT>::apply(act);
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template join<ACT>::type
    >::type
    join(boost::detail::thread_move_t<ACT> act) {
        return partial_specialization_workaround::join<boost::detail::thread_move_t<ACT> >::apply(act);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

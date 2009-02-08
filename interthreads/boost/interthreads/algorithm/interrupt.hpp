//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_INTERRUPT__HPP
#define BOOST_INTERTHREADS_INTERRUPT__HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace result_of {
        template <typename ACT>
        struct interrupt {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename ACT >
        struct interrupt {
            static typename result_of::template interrupt<ACT>::type apply( ACT& act ) {
                return act.interrupt();
            }
        };
        template< typename ACT >
        struct interrupt<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template interrupt<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                return act->interrupt();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        void
    >::type
    interrupt(ACT& act) {
        return partial_specialization_workaround::interrupt<ACT>::apply(act);
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        void
    >::type
    interrupt(boost::detail::thread_move_t<ACT> act) {
        return partial_specialization_workaround::interrupt<boost::detail::thread_move_t<ACT> >::apply(act);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

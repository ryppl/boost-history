//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_INTERRUPTION_REQUESTED__HPP
#define BOOST_INTERTHREADS_INTERRUPTION_REQUESTED__HPP

#include <boost/fusion/include/all.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/config/abi_prefix.hpp>

    namespace boost {
    namespace interthreads {

        namespace result_of {
            template <typename ACT>
            struct interruption_requested {
                typedef bool type;
            };
        }

        namespace partial_specialization_workaround {
            template< typename ACT >
            struct interruption_requested {
                static typename result_of::template interruption_requested<ACT>::type apply( ACT& act ) {
                    return act.interruption_requested();
                }
            };
            template< typename ACT >
            struct interruption_requested<boost::detail::thread_move_t<ACT> > {
                static typename result_of::template interruption_requested<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                    return act->interruption_requested();
                }
            };
        }

        template <typename ACT>
        typename boost::enable_if<has_thread_if<ACT>,
            bool
        >::type
        interruption_requested(ACT& act) {
            return partial_specialization_workaround::interruption_requested<ACT>::apply(act);
        }

        template <typename ACT>
        typename boost::enable_if<has_thread_if<ACT>,
            bool
        >::type
        interruption_requested(boost::detail::thread_move_t<ACT> act) {
            return partial_specialization_workaround::interruption_requested<boost::detail::thread_move_t<ACT> >::apply(act);
        }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_JOINABLE__HPP
#define BOOST_ASYNC_JOINABLE__HPP

#include <boost/fusion/include/all.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace result_of {
        template <typename ACT>
        struct joinable {
            typedef bool type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename ACT> struct joinable {
            static typename result_of::template joinable<ACT>::type apply( ACT& act ) {
                return act.joinable();
            }
        };
        template <typename ACT> struct joinable<boost::detail::thread_move_t<ACT> >{
            static typename result_of::template joinable<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                return act->joinable();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template joinable<ACT>::type
    >::type joinable(ACT& act) {
        return partial_specialization_workaround::joinable<ACT>::apply(act);
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,
        typename result_of::template joinable<ACT>::type
    >::type joinable(boost::detail::thread_move_t<ACT> act) {
        return partial_specialization_workaround::joinable<boost::detail::thread_move_t<ACT> >::apply(act);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

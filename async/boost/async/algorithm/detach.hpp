//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_DETACH__HPP
#define BOOST_ASYNC_DETACH__HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace result_of {
        template <typename ACT> struct detach {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename ACT> struct detach {
            static typename result_of::template detach<ACT>::type apply( ACT& act ) {
                return act.detach();
            }
        };

        template <typename ACT> struct detach<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template detach<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                return act->detach();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,void>::type
    detach(ACT& act) {
        return partial_specialization_workaround::detach<ACT>::apply(act);
    }

    template <typename ACT>
    typename boost::enable_if<has_thread_if<ACT>,void>::type
    detach(boost::detail::thread_move_t<ACT> act) {
        return partial_specialization_workaround::detach<boost::detail::thread_move_t<ACT> >::apply(act);
    }


}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

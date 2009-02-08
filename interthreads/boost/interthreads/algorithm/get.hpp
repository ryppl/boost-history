//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_GET__HPP
#define BOOST_INTERTHREADS_GET__HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/transform.hpp>
#include  <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace result_of {
        template <typename ACT> struct get {
            typedef typename act_traits<ACT>::move_dest_type type;
        };
        template <typename ACT> struct get<boost::detail::thread_move_t<ACT> > {
            typedef typename act_traits<ACT>::move_dest_type type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename ACT> struct get {
            static typename result_of::template get<ACT>::type apply( ACT& act ) {
                return act.get();
            }
        };

        template <typename ACT> struct get<boost::detail::thread_move_t<ACT> > {
            static typename result_of::template get<ACT>::type apply( boost::detail::thread_move_t<ACT> act ) {
                return act->get();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template get<ACT>::type
    >::type
    get(ACT& act) {
        return partial_specialization_workaround::get<ACT>::apply(act);
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template get<ACT>::type
    >::type
    get(boost::detail::thread_move_t<ACT> act) {
        return partial_specialization_workaround::get<ACT>::apply(act);
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

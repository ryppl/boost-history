//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_IS_READY__HPP
#define BOOST_INTERTHREADS_IS_READY__HPP

#include <boost/fusion/include/all.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace result_of {
        template <typename ACT> struct is_ready {
            typedef bool type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename ACT> struct is_ready {
            static typename result_of::is_ready<ACT>::type apply( ACT& act ) {
                return act.is_ready();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template is_ready<ACT>::type
    >::type is_ready(ACT& act) {
        return partial_specialization_workaround::is_ready<ACT>::apply(act);
    }


}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

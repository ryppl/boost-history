//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_HAS_VALUE__HPP
#define BOOST_ASYNC_HAS_VALUE__HPP

#include <boost/fusion/include/all.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace result_of {
        template <typename ACT> struct has_value {
            typedef bool type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename ACT> struct has_value {
            static typename result_of::has_value<ACT>::type apply( ACT& act ) {
                return act.has_value();
            }
        };
    }

    template <typename ACT>
    typename boost::enable_if<has_future_if<ACT>,
        typename result_of::template is_ready<ACT>::type
    >::type has_value(ACT& act) {
        return partial_specialization_workaround::has_value<ACT>::apply(act);
    }


}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

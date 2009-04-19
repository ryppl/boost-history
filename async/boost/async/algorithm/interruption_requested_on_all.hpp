//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_INTERRUPTION_REQUESTED_ON_ALL__HPP
#define BOOST_ASYNC_INTERRUPTION_REQUESTED_ON_ALL__HPP

#include <boost/fusion/include/all.hpp>

#include <boost/async/algorithm/interruption_requested.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace async {

    namespace fct {
        struct interruption_requested {
            typedef bool result_type;

            template<typename ACT>
            bool operator()(ACT& act) const {
                return async::interruption_requested(act);
            }
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct interruption_requested_on_all {
            typedef typename fusion::result_of::all<Sequence, fct::interruption_requested>::type type;
        };
    }

    template <typename Sequence>
    bool interruption_requested_on_all(Sequence& t) {
        return fusion::all(t, fct::interruption_requested());
    }


}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

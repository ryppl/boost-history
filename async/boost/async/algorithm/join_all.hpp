//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_JOIN_ALL__HPP
#define BOOST_ASYNC_JOIN_ALL__HPP

#include <boost/fusion/include/for_each.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chrono/chrono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/async/algorithm/join.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace fct {
        struct join {
            typedef void result_type;
            template<typename ACT>
            void operator()(ACT& act) const {
                async::join(act);
            }
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct join_all {
            typedef typename fusion::result_of::for_each<Sequence, fct::join>::type type;
        };
    }

    template <typename Sequence>
    typename result_of::join_all<Sequence>::type
    join_all(Sequence& t) {
        return fusion::for_each(t, fct::join());
    }

}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

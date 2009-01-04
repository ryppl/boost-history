//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_JOIN_ALL__HPP
#define BOOST_INTERTHREADS_JOIN_ALL__HPP

#include <boost/fusion/include/for_each.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace fct {
        struct join {
            typedef void result_type;
            template<typename ACT>
            void operator()(ACT& act) const {
                act.join();
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

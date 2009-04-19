//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_WAIT_ALL__HPP
#define BOOST_ASYNC_WAIT_ALL__HPP

#include <boost/fusion/include/for_each.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chrono/chrono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/async/algorithm/wait.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/utility/enable_if.hpp>
//#include <boost/utility/disable_if.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace fct {
        struct wait {
            typedef void result_type;
            template<typename ACT>
            void operator()(ACT& act) const {
                async::wait(act);
            }
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct wait_all {
            typedef typename fusion::result_of::for_each<Sequence, fct::wait>::type type;
        };
    }


    template <typename Sequence>
    typename boost::enable_if<fusion::traits::is_sequence<Sequence>,
        typename result_of::template wait_all<Sequence>::type
    >::type
    wait_all(Sequence& t) {
        fusion::for_each(t, fct::wait());
    }

    template <typename ACT>
    typename boost::disable_if<fusion::traits::is_sequence<ACT>,
        typename result_of::template wait<ACT>::type
    >::type
    wait_all(ACT& t) {
        async::wait(t);
    }
}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_GET_ALL__HPP
#define BOOST_INTERTHREADS_GET_ALL__HPP

#include <boost/fusion/include/transform.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace fct {
        struct get {
            template<typename ACT>
            typename ACT::result_type operator()(ACT& act) const {
                return act.get();
            }
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct get_all {
            typedef typename fusion::result_of::transform<Sequence, fct::get>::type type;
        };
    }

    template <typename Sequence>
    typename result_of::get_all<Sequence>::type
    get_all(Sequence& t) {
        return fusion::transform(t, fct::get());
    }

}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

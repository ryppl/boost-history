//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_INTERRUPTION_REQUESTED_ON_ALL__HPP
#define BOOST_INTERTHREADS_INTERRUPTION_REQUESTED_ON_ALL__HPP

#include <boost/fusion/include/all.hpp>

#include <boost/interthreads/algorithm/joinable.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace interthreads {

    namespace fct {
        struct joinable {
            typedef bool result_type;

            template<typename ACT>
            bool operator()(ACT& act) const {
                return interthreads::joinable(act);
            }
        };
    }

    namespace result_of {
        template <typename Sequence>
        struct are_all_joinable {
            typedef typename fusion::result_of::all<Sequence, fct::joinable>::type type;
        };
    }

    template <typename Sequence>
    bool are_all_joinable(Sequence& t) {
        return fusion::all(t, joinable);
    }


}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

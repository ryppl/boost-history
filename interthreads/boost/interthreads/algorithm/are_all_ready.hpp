//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_ARE_ALL_READY__HPP
#define BOOST_INTERTHREADS_ARE_ALL_READY__HPP

#include <boost/fusion/include/all.hpp>

#include <boost/interthreads/algorithm/is_ready.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    namespace fct {
        struct is_ready {
            typedef bool result_type;

            template<typename ACT>
            bool operator()(ACT& act) const {
                return interthreads::is_ready(act);
            }
        };
    }

    namespace result_of {
        template <typename Sequence> struct are_all_ready {
            typedef typename fusion::result_of::all<Sequence, fct::is_ready>::type type;
        };
    }

    template <typename Sequence> bool are_all_ready(Sequence& t) {
        return fusion::all(t, fct::is_ready());
    }


}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

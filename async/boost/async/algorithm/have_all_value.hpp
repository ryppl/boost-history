//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASYNC_HAVE_ALL_VALUE__HPP
#define BOOST_ASYNC_HAVE_ALL_VALUE__HPP

#include <boost/fusion/include/all.hpp>

#include <boost/async/algorithm/has_value.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace async {

    namespace fct {
        struct has_value {
            typedef bool result_type;

            template<typename ACT> bool operator()(ACT& act) const {
                return async::has_value(act);
            }
        };
    }

    namespace result_of {
        template <typename Sequence> struct have_all_value {
            typedef typename fusion::result_of::all<Sequence, fct::has_value>::type type;
        };
    }

    template <typename Sequence> bool have_all_value(Sequence& t) {
        return fusion::all(t, fct::has_value());
    }


}
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

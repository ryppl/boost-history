//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_INTERRUPT_ALL__HPP
#define BOOST_INTERTHREADS_INTERRUPT_ALL__HPP

#include <boost/fusion/include/for_each.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
  
    namespace fct {
        struct interrupt {
            template<typename ACT>
            void operator()(ACT& act) const {
                act.interrupt();
            }
        };
    }
    
    namespace result_of {
        template <typename Sequence>
        struct interrupt_all {
            typedef typename fusion::result_of::for_each<Sequence, fct::interrupt>::type type;
        };
    }

    template <typename Sequence>
    void interrupt_all(Sequence& t) {
        fusion::for_each(t, fct::interrupt());
    }

}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif

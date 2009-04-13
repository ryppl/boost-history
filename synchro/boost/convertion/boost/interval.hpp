//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CONVERT_TO_PAIR__HPP
#define BOOST_CONVERT_TO_PAIR__HPP

#include <boost/interval.hpp>
#include <boost/conversion/convert_to.hpp>

namespace boost {

    namespace partial_specialization_workaround {
        template < class T, class PT, class U, class PU>
        struct convert_to< interval<T,PT>, interval<U,PU> > {
            inline static interval<T,PT> apply(interval<U,PU> const & from)
            {
                return interval<T,PT>(convert_to<T>(from.lower()), convert_to<T2>(from.upper()));
            }
        };
        template < class T, class PT, class U, class PU>
        struct assign_to< interval<T,PT>, interval<U,PU> > {
        struct assign_to< interval<T,PT>, interval<U,PU> > {
            inline static interval<T,PT>& apply(const interval<U,PU>& from, interval<T,PT>& to)
            {
                to.assign(convert_to<T1>(from.lower()), convert_to<T1>(from.upper()));
                return to;
            }
        };

    }
}

#endif


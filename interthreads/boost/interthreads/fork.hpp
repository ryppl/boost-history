#ifndef BOOST_INTERTHREADS_FORK__HPP
#define BOOST_INTERTHREADS_FORK__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/futures/future.hpp>
#include <boost/interthreads/act_traits.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {



namespace result_of {
    template <typename AE,typename F>
    struct fork {
        typedef typename boost::result_of<F()>::type result_type;
        typedef typename asynchronous_completion_token<AE, result_type>::type type;
    };
}

namespace partial_specialization_workaround {
template< typename AE, typename F >
struct fork {
    static typename result_of::fork<AE,F>::type apply(AE& ae, F fn ) {
        return ae.fork(fn);
    }
};
}

template< typename AE, typename F >
//    typename boost::disable_if<is_movable_only<ACT>,
        typename result_of::fork<AE,F>::type
//    >::type
fork( AE& ae, F fn ) {
    return partial_specialization_workaround::fork<AE,F>::apply(ae,fn);
}

template< typename AE, typename F, typename A1 >
typename asynchronous_completion_token<AE, typename boost::result_of<F(A1)>::type >::type
fork( AE& ae, F fn, A1 a1 ) {
    return ae.fork( bind( fn, a1 ) );
}

template< typename AE, typename F, typename A1, typename A2 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2)>::type >::type
fork( AE& ae, F fn, A1 a1, A2 a2 ) {
    return ae.fork( bind( fn, a1, a2 ) );
}

template< typename AE, typename F, typename A1, typename A2, typename A3 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2,A3)>::type >::type
fork( AE& ae, F fn, A1 a1, A2 a2, A3 a3 ) {
    return ae.fork( bind( fn, a1, a2, a3 ) );
}

template< typename AE, typename F, typename A1, typename A2, typename A3, typename A4 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2,A3,A4)>::type >::type
fork( AE& ae, F fn, A1 a1, A2 a2, A3 a3, A4 a4 ) {
    return ae.fork( bind( fn, a1, a2, a3, a4 ) );
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


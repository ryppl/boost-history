#ifndef BOOST_INTERTHREADS_FORK_ALL__HPP
#define BOOST_INTERTHREADS_FORK_ALL__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/fork.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

    
namespace result_of { 
    template <typename AE, typename T>
    struct fork_all;
    template <typename AE, typename F1>
    struct fork_all<AE, fusion::tuple<F1> > {
        typedef  fusion::tuple<
            typename result_of::fork<AE,F1>::type
        > type;
    };
    template <typename AE, typename F1, typename F2>
    struct fork_all<AE, fusion::tuple<F1,F2> > {
        typedef  fusion::tuple<
            typename result_of::fork<AE,F1>::type,
            typename result_of::fork<AE,F2>::type
        > type;
    };
    template <typename AE, typename F1, typename F2, typename F3>
    struct fork_all<AE, fusion::tuple<F1,F2,F3> > {
        typedef  fusion::tuple<
            typename fork<AE,F1>::type,
            typename fork<AE,F2>::type,
            typename fork<AE,F3>::type
        > type;
    };
    
}


template< typename AE, typename F1> 
typename result_of::fork_all<AE, fusion::tuple<F1> >::type
fork_all( AE& ae, F1 f1 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1> >::type type;
    typename result_of::fork<AE, F1>::type j1 =ae.fork(f1);
    return type(j1);
}

template< typename AE, typename F1, typename F2> 
typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type
fork_all( AE& ae, F1 f1, F2 f2 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type type;
    typename result_of::fork<AE, F1>::type j1 =ae.fork(f1);
    typename result_of::fork<AE, F2>::type j2 =ae.fork(f2);
    return type(j1,j2);
}

template< typename AE, typename F1, typename F2, typename F3> 
typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type
fork_all( AE& ae, F1 f1, F2 f2, F3 f3 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type type;
    return type(ae.fork(f1),ae.fork(f2),ae.fork(f3));
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


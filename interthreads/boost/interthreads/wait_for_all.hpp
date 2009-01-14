#ifndef BOOST_INTERTHREADS_WAIT_FOR_ALL__HPP
#define BOOST_INTERTHREADS_WAIT_FOR_ALL__HPP

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

#include <boost/interthreads/fork_all.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

namespace result_of { 
    template <typename L, typename T>
    struct wait_for_all;
    template <typename L, typename F1, typename F2>
    struct wait_for_all<L,fusion::tuple<F1,F2> > {
        typedef  fusion::tuple<
            typename boost::result_of<F1()>::type,
            typename boost::result_of<F2()>::type
        > type;
    };
    template <typename L, typename F1, typename F2, typename F3>
    struct wait_for_all<L, fusion::tuple<F1,F2,F3> > {
        typedef  fusion::tuple<
            typename boost::result_of<F1()>::type,
            typename boost::result_of<F2()>::type,
            typename boost::result_of<F3()>::type
        > type;
    };
    
}


template< typename Launcher, typename F1, typename F2> 
typename result_of::wait_for_all<Launcher, fusion::tuple<F1,F2> >::type
wait_for_all( Launcher& launcher, F1 f1, F2 f2 ) {
    typename result_of::fork_all<Launcher, fusion::tuple<F1,F2> >::type tmp=fork_all(launcher, f1, f2);
    return get_all(tmp);
}

template< typename Launcher, typename F1, typename F2, typename F3> 
typename result_of::wait_for_all<Launcher, fusion::tuple<F1,F2,F3> >::type
wait_for_all( Launcher& launcher, F1 f1, F2 f2 , F3 f3 ) {
    typename result_of::fork_all<Launcher, fusion::tuple<F1,F2> >::type tmp=fork_all(launcher, f1, f2, f3);
    return get_all(tmp);
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


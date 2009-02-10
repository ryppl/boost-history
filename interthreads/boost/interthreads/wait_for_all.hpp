#ifndef BOOST_INTERTHREADS_WAIT_FOR_ALL__HPP
#define BOOST_INTERTHREADS_WAIT_FOR_ALL__HPP

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

#include <boost/interthreads/fork_all.hpp>
#include <boost/interthreads/algorithm/get_all.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

namespace result_of {
    template <typename L, typename T>
    struct wait_for_all;
    template <typename L, typename F1>
    struct wait_for_all<L,fusion::tuple<F1> > {
        typedef  fusion::tuple<
            typename boost::result_of<F1()>::type
        > type;
    };
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
    template <typename L, typename F1, typename F2, typename F3, typename F4>
    struct wait_for_all<L, fusion::tuple<F1,F2,F3,F4> > {
        typedef  fusion::tuple<
            typename boost::result_of<F1()>::type,
            typename boost::result_of<F2()>::type,
            typename boost::result_of<F3()>::type,
            typename boost::result_of<F4()>::type
        > type;
    };

}

template< typename AE, typename F1>
typename result_of::wait_for_all<AE, fusion::tuple<F1> >::type
wait_for_all( AE& ae, F1 f1 ) {
    #if 1
    typename result_of::fork_all<AE, fusion::tuple<F1> >::type handles(fork_all(ae, f1));
    typename result_of::wait_for_all<AE, fusion::tuple<F1> >::type values;
    set_all(handles,values);
    return values;
    #else
    //typename boost::result_of<F1()>::type = f1();
    return typename result_of::wait_for_all<AE, fusion::tuple<F1> >::type(f1());
    #endif
}

template< typename AE, typename F1, typename F2>
typename result_of::wait_for_all<AE, fusion::tuple<F1,F2> >::type
wait_for_all( AE& ae, F1 f1, F2 f2 ) {
    #if 1
    typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type handles(fork_all(ae, f1, f2));
    typename result_of::wait_for_all<AE, fusion::tuple<F1,F2> >::type values;
    set_all(handles,values);
    return values;
    #else
    typename result_of::fork_all<AE, fusion::tuple<F1> >::type handles(fork_all(ae, f1));
    typename result_of::wait_for_all<AE, fusion::tuple<F1> >::type values;
    set_all(handles,values);
    return typename result_of::wait_for_all<AE, fusion::tuple<F1,F2> >::type(
        fusion::at_c<0>(values), f2());
    #endif
}

#if 0
template< typename AE, typename F, typename Sequence>
typename result_of::wait_for_all<AE, fusion::tuple<F1,F2> >::type
wait_for_all( AE& ae, F f, const Sequence& seq ) {
    typename result_of::fork_all_seq<AE, F, Sequence>::type handles(fork_all(ae, f, seq));
    typename result_of::wait_for_all_seq<AE, F, Sequence> >::type values;
    set_all(handles,values);
    return values;
}
#endif

template< typename AE, typename F1, typename F2, typename F3>
typename result_of::wait_for_all<AE, fusion::tuple<F1,F2,F3> >::type
wait_for_all( AE& ae, F1 f1, F2 f2 , F3 f3 ) {
    #if 1
    typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type handles(fork_all(ae, f1, f2, f3));
    typename result_of::wait_for_all<AE, fusion::tuple<F1,F2,F3> >::type values;
    set_all(handles,values);
    return values;
    #else
    typename result_of::fork_all<AE, fusion::tuple<F1, F2> >::type handles(fork_all(ae, f1, f2));
    typename result_of::wait_for_all<AE, fusion::tuple<F1, F2> >::type values;
    set_all(handles,values);
    return typename result_of::wait_for_all<AE, fusion::tuple<F1,F2, F3> >::type(
        fusion::at_c<0>(values), fusion::at_c<1>(values), f3());
    #endif
}
template< typename AE, typename F1, typename F2, typename F3, typename F4>
typename result_of::wait_for_all<AE, fusion::tuple<F1,F2,F3,F4> >::type
wait_for_all( AE& ae, F1 f1, F2 f2 , F3 f3, F4 f4 ) {
    #if 1
    typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type handles(fork_all(ae, f1, f2, f3, f4));
    typename result_of::wait_for_all<AE, fusion::tuple<F1,F2,F3> >::type values;
    set_all(handles,values);
    return values;
    #else
    typename result_of::fork_all<AE, fusion::tuple<F1, F2> >::type handles(fork_all(ae, f1, f2, f3));
    typename result_of::wait_for_all<AE, fusion::tuple<F1, F2> >::type values;
    set_all(handles,values);
    return typename result_of::wait_for_all<AE, fusion::tuple<F1,F2, F3, F4> >::type(
        fusion::at_c<0>(values), fusion::at_c<1>(values), fusion::at_c<2>(values), f4());
    #endif
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


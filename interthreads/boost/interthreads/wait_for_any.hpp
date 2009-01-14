#ifndef BOOST_INTERTHREADS_WAIT_FOR_ANY__HPP
#define BOOST_INTERTHREADS_WAIT_FOR_ANY__HPP

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
#include <exception>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

namespace result_of { 
    template <typename L, typename T>
    struct wait_for_any;
    template <typename L, typename F1, typename F2>
    struct wait_for_any<L, fusion::tuple<F1,F2> > {
        typedef  std::pair<unsigned,typename boost::result_of<F1()>::type> type;
    };
    template <typename L, typename F1, typename F2, typename F3>
    struct wait_for_any<L, fusion::tuple<F1,F2,F3> > {
        typedef  std::pair<unsigned,typename boost::result_of<F1()>::type> type;
    };
    
}


template< typename AE, typename F1, typename F2> 
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type
wait_for_any( AE& ae, F1 f1, F2 f2 ) {
    typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type tmp=fork_all(ae, f1, f2);
    unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(tmp)), get_future<AE>()(fusion::at_c<1>(tmp)));
    typename boost::result_of<F1()>::type val;
    switch (r) {
        case 0:
            val = fusion::at_c<0>(tmp).get();
            break;
        case 1:
            val = fusion::at_c<1>(tmp).get();
            break;
        default:
            std::cout << "boost::wait_for_any=" << r << std::endl;
            throw std::range_error("");
    }
    interrupt_all(tmp);
    return std::make_pair(r, val);
}

template< typename AE, typename F1, typename F2, typename F3> 
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type
wait_for_any( AE& ae, F1 f1, F2 f2 , F3 f3 ) {
    typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type tmp=fork_all(ae, f1, f2, f3);
    unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(tmp)), get_future<AE>()(fusion::at_c<1>(tmp)), get_future<AE>()(fusion::at_c<2>(tmp)));
    typename boost::result_of<F1()>::type val;
    switch (r) {
        case 0:
            val = fusion::at_c<0>(tmp).get();
        case 1:
            val = fusion::at_c<1>(tmp).get();
        case 2:
            val = fusion::at_c<2>(tmp).get();
        default:
            std::cout << "boost::wait_for_any=" << r << std::endl;
            //throw std::range_error("");
    }
    return std::make_pair(r, val);
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


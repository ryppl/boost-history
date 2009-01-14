#ifndef BOOST_INTERTHREADS_WAIT_FOR_ANY__HPP
#define BOOST_INTERTHREADS_WAIT_FOR_ANY__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/algorithm/interrupt_all.hpp>
#include <boost/interthreads/fork_all.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>
#include <exception>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

namespace detail {
    
    template <typename T>
    struct pair_void {
        typedef std::pair<unsigned,T> type;
        typedef T value_type;
    };
    template <>
    struct pair_void<void> {
        typedef std::pair<unsigned,unsigned> type;
        typedef void value_type;
    };

    template <unsigned size, typename T> 
    struct partial;
    template <typename P> 
    struct partial<2,P> {
#if 1        
        typedef typename pair_void<P>::value_type T;
        typedef typename pair_void<P>::type type;
#else        
        typedef P T;
        typedef std::pair<unsigned, P> type;
#endif        
        template <typename H>
        static type make(unsigned r, H &handles) {
            return std::make_pair(r,get(r,handles));
        }
        template <typename H>
        static T get(unsigned r, H &handles) {
            switch (r) {
            case 0:
                return fusion::at_c<0>(handles).get();
            case 1:
                return fusion::at_c<1>(handles).get();
            default:
                throw std::range_error("");
            }
        }
    };
    template <typename P> 
    struct partial<3,P> {
#if 1        
        typedef typename pair_void<P>::value_type T;
        typedef typename pair_void<P>::type type;
#else        
        typedef P T;
        typedef std::pair<unsigned, P> type;
#endif
        template <typename H>
        static type make(unsigned r, H &handles) {
            return std::make_pair(r,get(r,handles));
        }
        template <typename H>
        static T get(unsigned r, H &handles) {
            switch (r) {
            case 0:
                return fusion::at_c<0>(handles).get();
            case 1:
                return fusion::at_c<1>(handles).get();
            case 2:
                return fusion::at_c<2>(handles).get();
            default:
                throw std::range_error("");
            }
        }
    };
    
    struct partial_aux {
        typedef pair_void<void>::value_type T;
        typedef pair_void<void>::type type;
        //typedef void T;
        //typedef std::pair<unsigned,unsigned> type;
        //typedef std::make_pair<unsigned, P> type;

        template <typename H>
        static type make(unsigned i,H &h) {
            return std::make_pair(i,i);
        }
    };
        
    template <> 
    struct partial<2,  void > : partial_aux{};
        
    template <> 
    struct partial<3,  void > : partial_aux{};
}    
    
namespace result_of { 
    template <typename L, typename T>
    struct wait_for_any;
    template <typename L, typename F1, typename F2>
    struct wait_for_any<L, fusion::tuple<F1,F2> > {
        typedef typename detail::pair_void<typename boost::result_of<F1()>::type>::type type;
    };
    template <typename L, typename F1, typename F2, typename F3>
    struct wait_for_any<L, fusion::tuple<F1,F2,F3> > {
        typedef typename detail::pair_void<typename boost::result_of<F1()>::type>::type type;
    };
    
}


template< typename AE, typename F1, typename F2> 
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type
wait_for_any( AE& ae, F1 f1, F2 f2 ) {
    typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type handles=fork_all(ae, f1, f2);
    unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)));
    std::cout << "boost::wait_for_any=" << r << std::endl;
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type res = 
        detail::partial<2,typename boost::result_of<F1()>::type >::make(r,handles);
    
    return res;
}

template< typename AE, typename F1, typename F2, typename F3> 
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type
wait_for_any( AE& ae, F1 f1, F2 f2 , F3 f3 ) {
    typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type handles=fork_all(ae, f1, f2, f3);
    unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)), get_future<AE>()(fusion::at_c<2>(handles)));
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type res = 
        detail::partial<3, typename boost::result_of<F1()>::type >::make(r,handles);
    return res;
}

template< typename AE, typename F1, typename F2> 
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type
wait_for_any_and_interrupt( AE& ae, F1 f1, F2 f2 ) {
    typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type handles=fork_all(ae, f1, f2);
    unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)));
    std::cout << "boost::wait_for_any=" << r << std::endl;
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type res = 
        detail::partial<2,typename boost::result_of<F1()>::type >::make(r,handles);
    
    interrupt_all(handles);
    return res;
}

template< typename AE, typename F1, typename F2, typename F3> 
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type
wait_for_any_and_interrupt( AE& ae, F1 f1, F2 f2 , F3 f3 ) {
    typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type handles=fork_all(ae, f1, f2, f3);
    unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)), get_future<AE>()(fusion::at_c<2>(handles)));
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type res = 
        detail::partial<3, typename boost::result_of<F1()>::type >::make(r,handles);
    interrupt_all(handles);
    return res;
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


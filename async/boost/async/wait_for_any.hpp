#ifndef BOOST_ASYNC_WAIT_FOR_ANY__HPP
#define BOOST_ASYNC_WAIT_FOR_ANY__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/async/algorithm/interrupt_all.hpp>
#include <boost/async/algorithm/wait_all.hpp>
#include <boost/async/algorithm/get.hpp>
#include <boost/async/fork_all.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <exception>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace async {

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
        typedef typename pair_void<P>::value_type T;
        typedef typename pair_void<P>::type type;
        template <typename H>
        static type make(unsigned r, H &handles) {
            return std::make_pair(r,get(r,handles));
        }
        template <typename H>
        static T get(unsigned r, H &handles) {
            switch (r) {
            case 0:
                return async::get(fusion::at_c<0>(handles));
            case 1:
                return async::get(fusion::at_c<1>(handles));
            default:
                throw std::range_error("");
            }
        }
    };
    template <typename P>
    struct partial<3,P> {
        typedef typename pair_void<P>::value_type T;
        typedef typename pair_void<P>::type type;
        template <typename H>
        static type make(unsigned r, H &handles) {
            return std::make_pair(r,get(r,handles));
        }
        template <typename H>
        static T get(unsigned r, H &handles) {
            switch (r) {
            case 0:
                return async::get(fusion::at_c<0>(handles));
            case 1:
                return async::get(fusion::at_c<1>(handles));
            case 2:
                return async::get(fusion::at_c<2>(handles));
            default:
                throw std::range_error("");
            }
        }
    };

    struct partial_aux {
        typedef pair_void<void>::value_type T;
        typedef pair_void<void>::type type;

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
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type handles_type;
    handles_type handles=fork_all(ae, f1, f2);
    //unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,0>::type
        >::type >::move_dest_type
    >::type fut0= get_future<AE>()(fusion::at_c<0>(handles));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,1>::type
        >::type >::move_dest_type
    >::type fut1= get_future<AE>()(fusion::at_c<1>(handles));
    unsigned r = boost::wait_for_any(fut0, fut1);
    //std::cout << "boost::wait_for_any=" << r << std::endl;
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type res =
        detail::partial<2,typename boost::result_of<F1()>::type >::make(r,handles);
    //interrupt_all(handles);
    //wait_all(handles);

    return res;
}

template< typename AE, typename F1, typename F2, typename F3>
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type
wait_for_any( AE& ae, F1 f1, F2 f2 , F3 f3 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type handles_type;
    handles_type handles=fork_all(ae, f1, f2, f3);
    //unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)), get_future<AE>()(fusion::at_c<2>(handles)));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,0>::type
        >::type >::move_dest_type
    >::type fut0= get_future<AE>()(fusion::at_c<0>(handles));
    typename get_future<AE>::template future_type<
        typename act_traits<  typename remove_reference<
            typename fusion::result_of::at_c<handles_type,1>::type
        >::type >::move_dest_type
    >::type fut1= get_future<AE>()(fusion::at_c<1>(handles));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,2>::type
        >::type >::move_dest_type
    >::type fut2= get_future<AE>()(fusion::at_c<2>(handles));
    unsigned r = boost::wait_for_any(fut0, fut1, fut2);
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type res =
        detail::partial<3, typename boost::result_of<F1()>::type >::make(r,handles);
    //interrupt_all(handles);
    return res;
}

template< typename AE, typename F1, typename F2>
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type
wait_for_any_and_interrupt( AE& ae, F1 f1, F2 f2 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type handles_type;
    handles_type handles=fork_all(ae, f1, f2);
    //unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,0>::type
        >::type >::move_dest_type
    >::type fut0= get_future<AE>()(fusion::at_c<0>(handles));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,1>::type
        >::type >::move_dest_type
    >::type fut1= get_future<AE>()(fusion::at_c<1>(handles));
    unsigned r = boost::wait_for_any(fut0, fut1);
    std::cout << "boost::wait_for_any=" << r << std::endl;
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2> >::type res =
        detail::partial<2,typename boost::result_of<F1()>::type >::make(r,handles);

    interrupt_all(handles);
    return res;
}

template< typename AE, typename F1, typename F2, typename F3>
typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type
wait_for_any_and_interrupt( AE& ae, F1 f1, F2 f2 , F3 f3 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type handles_type;
    handles_type handles=fork_all(ae, f1, f2, f3);
    //unsigned r = boost::wait_for_any(get_future<AE>()(fusion::at_c<0>(handles)), get_future<AE>()(fusion::at_c<1>(handles)), get_future<AE>()(fusion::at_c<2>(handles)));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,0>::type
        >::type >::move_dest_type
    >::type fut0= get_future<AE>()(fusion::at_c<0>(handles));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,1>::type
        >::type >::move_dest_type
    >::type fut1= get_future<AE>()(fusion::at_c<1>(handles));
    typename get_future<AE>::template future_type<
        typename act_traits< typename remove_reference<
            typename fusion::result_of::at_c<handles_type,2>::type
        >::type >::move_dest_type
    >::type fut2= get_future<AE>()(fusion::at_c<2>(handles));
    unsigned r = boost::wait_for_any(fut0, fut1, fut2);
    typename result_of::wait_for_any<AE, fusion::tuple<F1,F2,F3> >::type res =
        detail::partial<3, typename boost::result_of<F1()>::type >::make(r,handles);
    interrupt_all(handles);
    return res;
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


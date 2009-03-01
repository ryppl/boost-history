#ifndef BOOST_INTERTHREADS_FORK_ALL__HPP
#define BOOST_INTERTHREADS_FORK_ALL__HPP

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

#include <boost/interthreads/fork.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

#if 0
    namespace fct {
        template <typename AE, typename F>
        struct fork {
            fork(AE& ae, F f);
            template <typename Sig>
            struct result;

            template <typename T>
            struct result<fork(T)>
            {
                typename asynchronous_completion_token<AE, typename boost::result_of<F(T)>::type >::type type;
            };

            template <typename T>
            typename typename asynchronous_completion_token<AE, typename boost::result_of<F(T)>::type >::type operator()(T& act) const {
                return typename asynchronous_completion_token<AE, typename boost::result_of<F(T)>::type >::type();
            }
        };

        template <typename AE, typename F>
        struct set_fork {
            set_fork(AE& ae, F f);
            template<typename Sig>
            struct result;

            template<typename T>
            struct result<set_fork(T)>
            {
                typedef void type;
            };

            template<typename PARAM_ACT>
            void operator()(PARAM_ACT param_act) const {
                fusion::at_c<1>(param_act)=interthreads::fork(fusion::at_c<0>(param_act));
            }
        };
    }

#endif
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
    template <typename AE, typename F1, typename F2, typename F3, typename F4>
    struct fork_all<AE, fusion::tuple<F1,F2,F3,F4> > {
        typedef  fusion::tuple<
            typename fork<AE,F1>::type,
            typename fork<AE,F2>::type,
            typename fork<AE,F3>::type,
            typename fork<AE,F4>::type
        > type;
    };

}


template< typename AE, typename F1>
typename result_of::fork_all<AE, fusion::tuple<F1> >::type
fork_all( AE& ae, F1 f1 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1> >::type type;
    typename result_of::fork<AE, F1>::type j1 = interthreads::fork(ae, f1);
    return type(j1);
}

template< typename AE, typename F1, typename F2>
typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type
fork_all( AE& ae, F1 f1, F2 f2 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2> >::type type;
    typename result_of::fork<AE, F1>::type j1 =interthreads::fork(ae, f1);
    typename result_of::fork<AE, F2>::type j2 =interthreads::fork(ae, f2);
    return type(j1,j2);
}

#if 0

    template <typename AE, typename F, typename SequenceHandles, typename SequenceValues>
    void set_fork_all(AE& ae, F f, SequenceValues& values, SequenceHandles& handles) {
        typedef fusion::vector<SequenceValues&, SequenceHandles&> sequences;
        sequences seqs(values, handles);
        fusion::zip_view<sequences> zip(seqs);
        fusion::for_each(zip, fct::set_fork_all(ae, f));
    }

template< typename AE, typename F, typename Sequence>
typename result_of::template fork_all_seq<AE, F, Sequence>::type
fork_all_apply( AE& ae, F f, Sequence seq ) {
    typename result_of::template fork_all_seq<AE, F, Sequence>::type res=
        fusion::as_vector(fusion::transform(seq, fct::fork(ae, f)));
    set_fork_all(seq, res);
    return res;
}
#endif

template< typename AE, typename F1, typename F2, typename F3>
typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type
fork_all( AE& ae, F1 f1, F2 f2, F3 f3 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3> >::type type;
    return type(interthreads::fork(ae, f1),interthreads::fork(ae, f2),interthreads::fork(ae, f3));
}

template< typename AE, typename F1, typename F2, typename F3, typename F4>
typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3,F4> >::type
fork_all( AE& ae, F1 f1, F2 f2, F3 f3, F4 f4 ) {
    typedef typename result_of::fork_all<AE, fusion::tuple<F1,F2,F3,F4> >::type type;
    return type(interthreads::fork(ae, f1),interthreads::fork(ae, f2),interthreads::fork(ae, f3),interthreads::fork(ae, f4));
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_DETAIL_FOR_EACH_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_DETAIL_FOR_EACH_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/distance.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename SeqRef, typename First, typename F>
    inline void
    for_each_linear(First const&, BOOST_FUSION_R_ELSE_LREF(F), mpl::true_)
    {
    }

    template <typename SeqRef, typename First, typename F>
    inline void
    for_each_linear(First const& first,
            BOOST_FUSION_R_ELSE_LREF(F) f,
            mpl::false_)
    {
        f(*first);

        detail::for_each_linear<SeqRef>(
                fusion::next(first),
                BOOST_FUSION_FORWARD(F,f),
                result_of::equal_to<
                    typename result_of::next<First>::type
                  , typename result_of::end<SeqRef>::type
                >());
    }


    template <typename Seq, typename F, typename Tag>
    inline void
    for_each(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            BOOST_FUSION_R_ELSE_LREF(F) f,
            Tag)
    {
        detail::for_each_linear<BOOST_FUSION_R_ELSE_LREF(Seq)>(
                fusion::begin(seq)
              , f
              , typename result_of::empty<Seq>::type());
    }

    template<int N>
    struct for_each_unrolled
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            f(fusion::deref(it0));

            typedef typename result_of::next<It0>::type It1;
            It1 it1(fusion::next(it0));
            f(fusion::deref(it1));

            typedef typename result_of::next<It1>::type It2;
            It2 it2(fusion::next(it1));
            f(fusion::deref(it2));

            typedef typename result_of::next<It2>::type It3;
            It3 it3(fusion::next(it2));
            f(fusion::deref(it3));

            for_each_unrolled<N-4>::call(fusion::next(it3), f);
        }
    };

    template<>
    struct for_each_unrolled<3>
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            f(fusion::deref(it0));

            typedef typename result_of::next<It0>::type It1;
            It1 it1(fusion::next(it0));
            f(fusion::deref(it1));

            typedef typename result_of::next<It1>::type It2;
            It2 it2(fusion::next(it1));
            f(fusion::deref(it2));
        }
    };

    template<>
    struct for_each_unrolled<2>
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            f(fusion::deref(it0));

            typedef typename result_of::next<It0>::type It1;
            It1 it1(fusion::next(it0));
            f(fusion::deref(it1));
        }
    };

    template<>
    struct for_each_unrolled<1>
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F))
        {
            f(fusion::deref(it0));
        }
    };

    template<>
    struct for_each_unrolled<0>
    {
        template<typename It, typename F>
        static void call(It const&, BOOST_FUSION_R_ELSE_LREF(F))
        {
        }
    };

    template <typename Seq, typename F>
    inline void
    for_each(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            BOOST_FUSION_R_ELSE_LREF(F) f,
            random_access_traversal_tag)
    {
        for_each_unrolled<result_of::empty<Seq>::value>::call(
                fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)),
                BOOST_FUSION_FORWARD(F,f));
    }
}}}


#endif

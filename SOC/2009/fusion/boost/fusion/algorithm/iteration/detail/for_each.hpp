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
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<int N>
    struct for_each_unrolled
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
        {
            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1(fusion::next(it0));
            typedef typename result_of::next<It1&>::type It2;
            It2 it2(fusion::next(it1));
            typedef typename result_of::next<It2&>::type It3;
            It3 it3(fusion::next(it2));

            f(fusion::deref(it0));
            f(fusion::deref(it1));
            f(fusion::deref(it2));
            f(fusion::deref(it3));

            for_each_unrolled<N-4>::call(
                    fusion::next(it3),
                    BOOST_FUSION_FORWARD(F,f));
        }
    };

    template<>
    struct for_each_unrolled<3>
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
        {
            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1(fusion::next(it0));

            f(fusion::deref(it0));
            f(fusion::deref(it1));
            f(fusion::deref(fusion::next(it1)));
        }
    };

    template<>
    struct for_each_unrolled<2>
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
        {
            f(fusion::deref(it0));
            f(fusion::deref(fusion::next(it0)));
        }
    };

    template<>
    struct for_each_unrolled<1>
    {
        template<typename It0, typename F>
        static void call(It0 const& it0, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
        {
            f(fusion::deref(it0));
        }
    };

    template<>
    struct for_each_unrolled<0>
    {
        template<typename It, typename F>
        static void call(It const&, BOOST_FUSION_RREF_ELSE_OBJ(F))
        {}
    };
}}}


#endif

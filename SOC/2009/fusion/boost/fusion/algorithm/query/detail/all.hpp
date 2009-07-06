/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_ALL_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_ALL_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>

#include <boost/mpl/bool.hpp>

//TODO code based on fold!!!

namespace boost { namespace fusion { namespace detail
{
    template <typename SeqRef, typename First, typename F>
    inline bool
    linear_all(First const&, BOOST_FUSION_R_ELSE_LREF(F), mpl::true_)
    {
        return true;
    }

    template <typename SeqRef,typename First, typename F>
    inline bool
    linear_all(First const& first, BOOST_FUSION_R_ELSE_LREF(F) f, mpl::false_)
    {
        return f(fusion::deref(first)) &&
            detail::linear_all(
                    fusion::next(first)
                  , BOOST_FUSION_FORWARD(F,f)
                  , result_of::equal_to<
                        typename result_of::next<First>::type,
                        typename result_of::end<SeqRef>::type
                    >::type());
    }

    template <typename Seq, typename F, typename Tag>
    inline bool
    all(BOOST_FUSION_R_ELSE_LREF(Seq) seq, BOOST_FUSION_R_ELSE_LREF(F) f, Tag)
    {
        return detail::linear_all<BOOST_FUSION_R_ELSE_LREF(Seq)>(
                fusion::begin(BOOST_FUSION_FORWARD(Seq,seq))
              , BOOST_FUSION_FORWARD(F,f)
              , result_of::empty<BOOST_FUSION_R_ELSE_LREF(Seq)>::type());
    }

    template<int N>
    struct unrolled_all
    {
        template <typename It, typename F>
        static bool call(It const& it, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            return
                f(fusion::deref(it)) &&
                f(fusion::deref(fusion::advance_c<1>(it)))&&
                f(fusion::deref(fusion::advance_c<2>(it))) &&
                f(fusion::deref(fusion::advance_c<3>(it))) &&
                detail::unrolled_all<N-4>::call(fusion::advance_c<4>(it), f);
        }
    };

    template<>
    struct unrolled_all<3>
    {
        template <typename It, typename F>
        static bool call(It const& it, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            return
                f(fusion::deref(it)) &&
                f(fusion::deref(fusion::advance_c<1>(it))) &&
                f(fusion::deref(fusion::advance_c<2>(it)));
        }
    };

    template<>
    struct unrolled_all<2>
    {
        template <typename It, typename F>
        static bool call(It const& it, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            return
                f(fusion::deref(it)) &&
                f(fusion::deref(fusion::advance_c<1>(it)));
        }
    };

    template<>
    struct unrolled_all<1>
    {
        template <typename It, typename F>
        static bool call(It const& it, BOOST_FUSION_R_ELSE_LREF(F) f)
        {
            return f(fusion::deref(it));
        }
    };

    template<>
    struct unrolled_all<0>
    {
        template <typename It, typename F>
        static bool call(It const& it, F const& f)
        {
            return false;
        }
    };

    template <typename Seq, typename F>
    inline bool
    all(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            BOOST_FUSION_R_ELSE_LREF(F) f,
            random_access_traversal_tag)
    {
        typedef
            detail::unrolled_all<
                result_of::size<BOOST_FUSION_R_ELSE_LREF(Seq)>::value
            >
        gen;

        return gen::call(
                fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)),
                BOOST_FUSION_FORWARD(F,f));
    }
}}}

#endif

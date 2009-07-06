/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_COUNT_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_COUNT_IF_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/advance.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion {
    struct random_access_traversal_tag;

    //code based on fold!!!

    namespace detail
    {
        template <typename SeqRef, typename First, typename F>
        inline int
        linear_count_if(First const&, BOOST_FUSION_R_ELSE_LREF(F), mpl::true_)
        {
            return 0;
        }

        template <typename SeqRef, typename First, typename F>
        inline int
        linear_count_if(
                First const& first,
                BOOST_FUSION_R_ELSE_LREF(F) f,
                mpl::false_)
        {
            int n = detail::linear_count_if(
                    fusion::next(first)
                  , f
                  , result_of::equal_to<
                        typename result_of::next<First>::type,
                        typename result_of::end<SeqRef>::type
                    >::value);
            if(f(*first))
            {
                ++n;
            }

            return n;
        }

        template <typename Seq, typename F, typename Tag>
        inline int
        count_if(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
                BOOST_FUSION_R_ELSE_LREF(F) f,
                Tag)
        {
            return detail::linear_count_if<BOOST_FUSION_R_ELSE_LREF(Seq)>(
                    fusion::begin(BOOST_FUSION_FORWARD(Seq, seq))
                  , BOOST_FUSION_FORWARD(F,f)
                  , result_of::empty<BOOST_FUSION_R_ELSE_LREF(Seq)>::type());
        }

        template<int n>
        struct unrolled_count_if
        {
            template<typename It0, typename F>
            static int call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                int ct = unrolled_count_if<n-4>::
                    call(fusion::advance_c<4>(it0), BOOST_FUSION_FORWARD(F,f));
                if(f(fusion::deref(it0)))
                {
                    ++ct;
                }

                typedef typename result_of::next<It0>::type It1;
                It1 it1(fusion::next(it0));
                if(f(fusion::deref(it1)))
                {
                    ++ct;
                }

                typedef typename result_of::next<It1>::type It2;
                It2 it2(fusion::next(it1));
                if(f(fusion::deref(it2)))
                {
                    ++ct;
                }

                typedef typename result_of::next<It2>::type It3;
                It3 it3(fusion::next(it2));
                if(f(fusion::deref(it3)))
                {
                    ++ct;
                }

                return ct;
            }
        };

        template<>
        struct unrolled_count_if<3>
        {
            template<typename It0, typename F>
            static int call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                int ct = 0;
                if(f(fusion::deref(it0)))
                {
                    ++ct;
                }

                typedef typename result_of::next<It0>::type It1;
                It1 it1(fusion::next(it0));
                if(f(fusion::deref(it1)))
                {
                    ++ct;
                }

                typedef typename result_of::next<It1>::type It2;
                It2 it2(fusion::next(it1));
                if(f(fusion::deref(it2)))
                {
                    ++ct;
                }

                return ct;
            }
        };

        template<>
        struct unrolled_count_if<2>
        {
            template<typename It0, typename F>
            static int call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                int ct = 0;
                if(f(fusion::deref(it0)))
                {
                    ++ct;
                }

                typedef typename result_of::next<It0>::type It1;
                It1 it1(fusion::next(it0));
                if(f(fusion::deref(it1)))
                {
                    ++ct;
                }

                return ct;
            }
        };

        template<>
        struct unrolled_count_if<1>
        {
            template<typename It0, typename F>
            static int call(It0 const& it0, BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                int ct = 0;
                if(f(fusion::deref(it0)))
                {
                    ++ct;
                }

                return ct;
            }
        };

        template<>
        struct unrolled_count_if<0>
        {
            template<typename It0, typename F>
            static int call(It0 const&, BOOST_FUSION_R_ELSE_LREF(F))
            {
                return 0;
            }
        };

        template <typename Seq, typename F>
        inline int
        count_if(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
                BOOST_FUSION_R_ELSE_LREF(F) f,
                random_access_traversal_tag)
        {
            typedef
                unrolled_count_if<
                    result_of::size<BOOST_FUSION_R_ELSE_LREF(Seq)>::value
                >
            gen;

            return gen::call(fusion::begin(seq),BOOST_FUSION_FORWARD(F,f));
        }
    }
}}

#endif

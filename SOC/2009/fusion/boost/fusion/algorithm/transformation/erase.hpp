/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_ERASE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_ERASE_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/iterator_range/iterator_range.hpp>
#include <boost/fusion/support/internal/as_fusion_element.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/algorithm/transformation/detail/is_in_seq.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename Seq, typename FirstRef>
        struct compute_erase_last
        {
            typedef typename result_of::end<Seq>::type last_type;

            typedef typename
                mpl::eval_if<
                    result_of::equal_to<FirstRef, last_type>
                  , mpl::identity<FirstRef>
                  , result_of::next<FirstRef>
                >::type
            type;

            static type
            call(FirstRef first, mpl::false_)
            {
                return fusion::next(first);
            }

            static type
            call(FirstRef first, mpl::true_)
            {
                return first;
            }

            static type
            call(FirstRef first)
            {
                return call(first,
                        result_of::equal_to<FirstRef, last_type>());
            }
        };
    }

    namespace result_of
    {
        template <
            typename Seq
          , typename First
          , typename Last =
                typename detail::compute_erase_last<
                    Seq
                  , typename detail::add_lref<First>::type
                >::type
        >
        struct erase
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef typename begin<Seq>::type seq_first_type;
            typedef typename end<Seq>::type seq_last_type;

            BOOST_FUSION_MPL_ASSERT(
                    (detail::is_in_range<First,seq_first_type,seq_last_type>));
            BOOST_FUSION_MPL_ASSERT(
                    (detail::is_in_range<Last,First,seq_last_type>));

            typedef
                iterator_range<
                    seq_first_type
                  , typename detail::identity<First>::type
                >
            left_type;
            typedef
                iterator_range<
                    typename detail::identity<Last>::type
                  , seq_last_type
                >
            right_type;

            typedef
                joint_view<left_type, right_type>
            type;
        };
    }

    template <typename Seq, typename First, typename Last>
    typename result_of::erase<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , First const&
      , Last const&
    >::type
    erase(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            First const& first,
            Last const& last)
    {
        typedef
            result_of::erase<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , First const&
              , Last const&
            >
        result;
        typedef typename result::left_type left_type;
        typedef typename result::right_type right_type;
        typedef typename result::type result_type;

        return result_type(
                left_type(
                    fusion::begin(seq)
                  , first
                )
              , right_type(
                    last
                  , fusion::end(seq)
                ));
    }

    template <typename Seq, typename First>
    typename result_of::erase<BOOST_FUSION_R_ELSE_CLREF(Seq), First const&>::type
    erase(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, First const& first)
    {
        return erase(
                BOOST_FUSION_FORWARD(Seq,seq)
              , first
              , detail::compute_erase_last<
                    BOOST_FUSION_R_ELSE_CLREF(Seq)
                  , First const&
                >::call(first));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename First, typename Last>
    typename result_of::erase<Seq&, First const&, Last const&>::type
    erase(Seq& seq,First const& first,Last const& last)
    {
        typedef result_of::erase<Seq&, First const&, Last const&> result;
        typedef typename result::left_type left_type;
        typedef typename result::right_type right_type;
        typedef typename result::type result_type;

        return result_type(
                left_type(
                    fusion::begin(seq)
                  , first
                )
              , right_type(
                    last
                  , fusion::end(seq)
                ));
    }

    template <typename Seq, typename First>
    typename result_of::erase<Seq&, First const&>::type
    erase(Seq& seq, First const& first)
    {
        return erase(
                seq
              , first
              , detail::compute_erase_last<Seq&, First const&>::call(first));
    }
#endif
}}

#endif

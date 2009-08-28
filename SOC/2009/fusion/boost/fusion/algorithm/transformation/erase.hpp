/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

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
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/algorithm/transformation/detail/is_in_seq.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename Seq, typename BeginRef>
        struct compute_erase_last
        {
            typedef typename result_of::end<Seq>::type end_type;

            typedef typename
                mpl::eval_if<
                    result_of::equal_to<BeginRef, end_type>
                  , mpl::identity<BeginRef>
                  , result_of::next<BeginRef>
                >::type
            type;

            static type
            call(BeginRef begin, mpl::false_)
            {
                return fusion::next(begin);
            }

            static type
            call(BeginRef begin, mpl::true_)
            {
                return begin;
            }

            static type
            call(BeginRef begin)
            {
                return call(begin,
                        result_of::equal_to<BeginRef, end_type>());
            }
        };
    }

    namespace result_of
    {
        template <
            typename Seq
          , typename Begin
          , typename End=typename detail::compute_erase_last<Seq, Begin>::type
        >
        struct erase
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<Begin>));
            BOOST_FUSION_MPL_ASSERT((detail::is_in_seq<Begin, Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<End>));
            BOOST_FUSION_MPL_ASSERT((
                detail::is_in_range<
                    End
                  , Begin
                  , typename result_of::end<Seq>::type
                >));

            typedef typename begin<Seq>::type seq_begin_type;
            typedef typename end<Seq>::type seq_end_type;
            typedef
                iterator_range<
                    seq_begin_type
                  , typename detail::identity<Begin>::type
                >
            left_type;
            typedef
                iterator_range<
                    typename detail::identity<End>::type
                  , seq_end_type
                >
            right_type;

            typedef
                joint_view<left_type, right_type>
            type;
        };
    }

    template <typename Seq, typename Begin, typename End>
    typename result_of::erase<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , Begin const&
      , End const&
    >::type
    erase(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            Begin const& first,
            End const& last)
    {
        typedef
            result_of::erase<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , Begin const&
              , End const&
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

    template <typename Seq, typename Begin>
    typename result_of::erase<BOOST_FUSION_R_ELSE_CLREF(Seq), Begin const&>::type
    erase(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, Begin const& first)
    {
        return erase(
                BOOST_FUSION_FORWARD(Seq,seq)
              , first
              , detail::compute_erase_last<
                    BOOST_FUSION_R_ELSE_CLREF(Seq)
                  , Begin const&
                >::call(first));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename Begin, typename End>
    typename result_of::erase<Seq&, Begin const&, End const&>::type
    erase(Seq& seq,Begin const& first,End const& last)
    {
        typedef result_of::erase<Seq&, Begin const&, End const&> result;
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

    template <typename Seq, typename Begin>
    typename result_of::erase<Seq&, Begin const&>::type
    erase(Seq& seq, Begin const& first)
    {
        return erase(
                seq
              , first
              , detail::compute_erase_last<Seq&, Begin const&>::call(first));
    }
#endif
}}

#endif

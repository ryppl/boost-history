/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_RANGE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_RANGE_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/iterator_range/iterator_range.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pos, typename Range>
        struct insert_range
        {
            typedef Pos pos_type;
            typedef typename result_of::begin<Seq>::type first_type;
            typedef typename result_of::end<Seq>::type last_type;

            typedef iterator_range<first_type, pos_type> left_type;
            typedef iterator_range<pos_type, last_type> right_type;
            typedef joint_view<left_type, Range> left_insert_type;
            typedef joint_view<left_insert_type, right_type> type;
        };
    }

    template <typename Seq, typename Pos, typename Range>
    inline typename
        result_of::insert_range<
            BOOST_FUSION_R_ELSE_LREF(Seq)
          , Pos const&
          , BOOST_FUSION_R_ELSE_LREF(Range)
        >::type
    insert_range(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            Pos const& pos,
            BOOST_FUSION_R_ELSE_LREF(Range) range)
    {
        typedef
            result_of::insert_range<
                BOOST_FUSION_R_ELSE_LREF(Seq)
              , Pos const&
              , BOOST_FUSION_R_ELSE_LREF(Range)
            >
        result_of;
        typedef typename result_of::left_type left_type;
        typedef typename result_of::right_type right_type;
        typedef typename result_of::left_insert_type left_insert_type;
        typedef typename result_of::type result;

        return result(
                left_insert_type(
                    left_type(
                        fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)),
                        pos),
                    BOOST_FUSION_FORWARD(Range,range)),
                right_type(
                    pos,
                    fusion::end(seq))
                );
    }
}}

#endif

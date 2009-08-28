/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_RANGE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_RANGE_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/iterator_range/iterator_range.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/algorithm/transformation/detail/is_in_seq.hpp>

#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pos, typename Range>
        struct insert_range
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<Pos>));
            BOOST_FUSION_MPL_ASSERT((detail::is_in_seq<Pos, Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Range>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Range>));

            typedef
                iterator_range<
                    typename begin<Seq>::type
                  , typename detail::identity<Pos>::type
                >
            left_type;
            typedef
                iterator_range<
                    typename detail::identity<Pos>::type
                  , typename end<Seq>::type
                >
            right_type;
            typedef
                joint_view<left_type, Range>
            left_insert_type;

            typedef joint_view<left_insert_type, right_type> type;
        };
    }

#define BOOST_FUSION_INSERT_RANGE(SEQ_CV_REF_MODIFIER,RANGE_CV_REF_MODIFIER)\
    template <typename Seq, typename Pos, typename Range>\
    inline typename\
        result_of::insert_range<\
            Seq SEQ_CV_REF_MODIFIER\
          , Pos const&\
          , Range RANGE_CV_REF_MODIFIER\
        >::type\
    insert_range(Seq SEQ_CV_REF_MODIFIER seq,\
            Pos const& pos,\
            Range RANGE_CV_REF_MODIFIER range)\
    {\
        typedef\
            result_of::insert_range<\
                Seq SEQ_CV_REF_MODIFIER\
              , Pos const&\
              , Range RANGE_CV_REF_MODIFIER\
            >\
        result;\
        typedef typename result::left_type left_type;\
        typedef typename result::right_type right_type;\
        typedef typename result::left_insert_type left_insert_type;\
        \
        return typename result::type(\
                left_insert_type(\
                    left_type(\
                        fusion::begin(seq),\
                        pos\
                    ),\
                    BOOST_FUSION_FORWARD(Range RANGE_CV_REF_MODIFIER,range)\
                ),\
                right_type(\
                    pos,\
                    fusion::end(seq)\
                )\
               );\
    }

    BOOST_FUSION_INSERT_RANGE(
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()),
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()));
#ifdef BOOST_NO_RVALUE_REFERENCES
    BOOST_FUSION_INSERT_RANGE(&,const&);
    BOOST_FUSION_INSERT_RANGE(const&,&);
    BOOST_FUSION_INSERT_RANGE(&,&);
#endif

#undef BOOST_FUSION_INSERT_RANGE
}}

#endif

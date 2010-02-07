/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_POP_FRONT_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_POP_FRONT_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/view/iterator_range/iterator_range.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq>
        struct pop_front
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT_NOT((empty<Seq>));

            typedef 
                iterator_range<
                    typename next<typename begin<Seq>::type>::type
                  , typename end<Seq>::type
                > 
            type;
        };
    }

    template <typename Seq>
    inline typename result_of::pop_front<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    pop_front(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename
            result_of::pop_front<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
            >::type(
                fusion::next(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq))),
                fusion::end(BOOST_FUSION_FORWARD(Seq,seq)));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq>
    inline typename result_of::pop_front<Seq&>::type
    pop_front(Seq& seq)
    {
        return typename
            result_of::pop_front<Seq&>::type(
                fusion::next(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq))),
                fusion::end(BOOST_FUSION_FORWARD(Seq,seq)));
    }
#endif
}}

#endif

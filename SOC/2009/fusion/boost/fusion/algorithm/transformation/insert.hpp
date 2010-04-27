/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_HPP

#include <boost/fusion/view/single_view/single_view.hpp>
#include <boost/fusion/algorithm/transformation/insert_range.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/algorithm/transformation/detail/is_in_seq.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pos, typename T>
        struct insert
          : insert_range<
                Seq
              , Pos
              , BOOST_FUSION_R_ELSE_CLREF(
                  single_view<typename traits::deduce<T>::type>)
            >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<Pos>));
            BOOST_FUSION_MPL_ASSERT((detail::is_in_seq<Pos, Seq>));
        };
    }

    template <typename Seq, typename Pos, typename T>
    inline typename
        result_of::insert<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , Pos const&
          , BOOST_FUSION_R_ELSE_CLREF(T)
        >::type
    insert(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            Pos const& pos,
            BOOST_FUSION_R_ELSE_CLREF(T) x)
    {
        return insert_range(
            BOOST_FUSION_FORWARD(Seq,seq),
            pos,
            fusion::make_single_view(BOOST_FUSION_FORWARD(T,x)));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename Pos, typename T>
    inline typename result_of::insert<Seq&, Pos const&, T const&>::type
    insert(Seq& seq,Pos const& pos,T const& x)
    {
        return insert_range(seq, pos, fusion::make_single_view(x));
    }
#endif
}}

#endif

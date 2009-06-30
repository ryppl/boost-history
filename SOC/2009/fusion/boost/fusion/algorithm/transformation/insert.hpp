/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_INSERT_HPP

#include <boost/fusion/view/single_view/single_view.hpp>
#include <boost/fusion/algorithm/transformation/insert_range.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pos, typename T>
        struct insert
          : insert_range<
                Seq
              , Pos
              //TODO cschmidt!!!
              , /*BOOST_FUSION_R_ELSE_CLREF(*/
                      single_view<typename detail::as_fusion_element<T>::type>//)
                  const&
            >
        {
        };
    }

    template <typename Seq, typename Pos, typename T>
    inline typename
        result_of::insert<
            BOOST_FUSION_R_ELSE_LREF(Seq)
          , Pos const&
          , BOOST_FUSION_R_ELSE_LREF(T)
        >::type
    insert(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            Pos const& pos,
            BOOST_FUSION_R_ELSE_LREF(T) x)
    {
        return insert_range(BOOST_FUSION_FORWARD(Seq,seq)
                , pos
                , (const single_view<T>&) make_single_view(BOOST_FUSION_FORWARD(T,x)));
    }

    //TODO cref
}}

#endif

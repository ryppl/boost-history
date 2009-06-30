/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_IF_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pred>
        struct find_if
        {
            typedef typename
                detail::static_find_if<
                    typename result_of::begin<Seq>::type
                  , typename result_of::end<Seq>::type
                  , Pred
                >::type
            type;
        };
    }

    template <typename Pred, typename Seq>
    inline typename
        result_of::find_if<BOOST_FUSION_R_ELSE_CLREF(Seq), Pred>::type
    find_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        typedef
            detail::static_find_if<
                typename result_of::begin<Seq>::type
              , typename result_of::end<Seq>::type
              , Pred
            >
        gen;

        return gen::call(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
    }
}}

#endif

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
            typedef
                detail::static_find_if<
                    typename result_of::begin<Seq>::type
                  , typename result_of::end<Seq>::type
                  , Pred
                >
            gen;

            typedef typename gen::type type;
        };
    }

    template <typename Pred, typename Seq>
    inline typename
        result_of::find_if<BOOST_FUSION_R_ELSE_CLREF(Seq), Pred>::type
    find_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::find_if<
                BOOST_FUSION_R_ELSE_CLREF(Seq), Pred
            >::gen::call(fusion::begin(seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Pred, typename Seq>
    inline typename result_of::find_if<Seq&, Pred>::type
    find_if(Seq& seq)
    {
        return result_of::find_if<Seq&, Pred>::gen::call(fusion::begin(seq));
    }
#endif
}}

#endif

/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_FILTER_IF_HPP

#include <boost/fusion/view/filter_view/filter_view.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pred>
        struct filter_if
        {
            typedef filter_view<Seq, Pred> type;
        };
    }
    
    template <typename Pred, typename Seq>
    inline typename
        result_of::filter_if<BOOST_FUSION_R_ELSE_CLREF(Seq), Pred>::type
    filter_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename
            result_of::filter_if<BOOST_FUSION_R_ELSE_CLREF(Seq), Pred>::type(
                BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Pred, typename Seq>
    inline typename result_of::filter_if<Seq&, Pred>::type
    filter_if(Seq& seq)
    {
        return typename result_of::filter_if<Seq&, Pred>::type(seq);
    }
#endif
}}

#endif

/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_POP_BACK_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_POP_BACK_HPP

#include <boost/fusion/view/iterator_range/iterator_range.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/prior.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq>
        struct pop_back
        {
            typedef 
                iterator_range<
                    typename begin<Seq>::type
                  , typename prior<typename end<Seq>::type>::type
                > 
            type;
        };
    }

    template <typename Seq>
    inline typename result_of::pop_back<BOOST_FUSION_R_ELSE_LREF(Seq)>::type
    pop_back(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        return typename
            result_of::pop_back<BOOST_FUSION_R_ELSE_LREF(Seq)>::type(
                    fusion::begin(seq)
                  , fusion::prior(fusion::end(seq)));
    }
}}

#endif

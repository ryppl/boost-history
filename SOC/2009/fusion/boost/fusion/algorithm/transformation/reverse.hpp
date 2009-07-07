/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REVERSE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REVERSE_HPP

#include <boost/fusion/view/reverse_view/reverse_view.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq>
        struct reverse
        {
            typedef reverse_view<Seq> type;
        };
    }

    template <typename Seq>
    inline typename result_of::reverse<BOOST_FUSION_R_ELSE_LREF(Seq)>::type
    reverse(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        return typename result_of::reverse<BOOST_FUSION_R_ELSE_LREF(Seq)>::type(
                BOOST_FUSION_FORWARD(Seq,seq));
    }
}}

#endif

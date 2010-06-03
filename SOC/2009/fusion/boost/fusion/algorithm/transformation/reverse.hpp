/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REVERSE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REVERSE_HPP

#include <boost/fusion/view/reverse_view/reverse_view.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename Seq>
        struct reverse
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef reverse_view<Seq> type;
        };
    }

    template<typename Seq>
    inline typename result_of::reverse<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    reverse(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename
            result_of::reverse<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type(
                BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Seq>
    inline typename result_of::reverse<Seq&>::type
    reverse(Seq& seq)
    {
        return typename result_of::reverse<Seq&>::type(seq);
    }
#endif
}}

#endif

/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_CLEAR_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_CLEAR_HPP

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/vector/vector.hpp>
#else
#   include <boost/fusion/container/vector/vector10.hpp>
#endif
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence>
        struct clear
        {
#ifdef BOOST_NO_VARIADIC_TEMPLATES
            typedef vector0 type;
#else
            typedef vector<> type;
#endif
        };
    }

    template <typename Seq>
    inline typename result_of::clear<BOOST_FUSION_R_ELSE_LREF(Seq)>::type
    clear(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        return typename result_of::clear<BOOST_FUSION_R_ELSE_LREF(Seq)>::type();
    }
}}

#endif

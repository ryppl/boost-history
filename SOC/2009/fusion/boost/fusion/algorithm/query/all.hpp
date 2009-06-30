/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_ALL_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_ALL_HPP

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/fusion/algorithm/query/detail/all.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename, typename>
        struct all
        {
            typedef bool type;
        };
    }

    template <typename Seq, typename F>
    inline bool
    all(BOOST_FUSION_R_ELSE_LREF(Seq) seq, BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return detail::all(
                BOOST_FUSION_FORWARD(Seq,seq),
                BOOST_FUSION_FORWARD(F,f),
                typename traits::category_of<
                    BOOST_FUSION_R_ELSE_LREF(Seq)
                >::type());
    }
}}

#endif

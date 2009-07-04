/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_COUNT_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_COUNT_HPP

#include <boost/fusion/algorithm/query/count_if.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/fusion/algorithm/query/detail/count.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename F>
        struct count
        {
            typedef int type;
        };
    }

    template <typename Seq, typename T>
    inline int
    count(BOOST_FUSION_R_ELSE_LREF(Seq) seq, BOOST_FUSION_R_ELSE_LREF(T) x)
    {
        return fusion::count_if(
                BOOST_FUSION_FORWARD(Seq,seq),
                detail::count_compare<BOOST_FUSION_R_ELSE_LREF(T)>(x));
    }
}}

#endif

/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_COUNT_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_COUNT_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/algorithm/query/count_if.hpp>

#include <boost/fusion/algorithm/query/detail/count.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename Seq, typename>
        struct count
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))

            typedef int type;
        };
    }

    template<typename Seq, typename T>
    inline typename result_of::count<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , BOOST_FUSION_R_ELSE_CLREF(T)
    >::type
    count(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_R_ELSE_CLREF(T) x)
    {
        return fusion::count_if(
            BOOST_FUSION_FORWARD(Seq,seq),
            detail::count_helper<BOOST_FUSION_R_ELSE_CLREF(T)>(
                BOOST_FUSION_FORWARD(T,x)));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename Seq, typename T>
    inline typename result_of::count<Seq&, T const&>::type
    count(Seq& seq, T const& x)
    {
        return fusion::count_if(seq,detail::count_helper<T const&>(x));
    }
#endif
}}

#endif

/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_COUNT_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_COUNT_IF_HPP

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename FRef>
        struct count_if_helper
        {
            typedef int result_type;

            count_if_helper(FRef f)
              : f(f)
            {}

            template<typename E>
            inline int
            operator()(BOOST_FUSION_R_ELSE_LREF(E) e, int count)
            {
                if(f(BOOST_FUSION_FORWARD(E,e)))
                {
                    return count+1;
                }

                return count;
            }

            FRef f;
        };
    }

    namespace result_of
    {
        template <typename Seq, typename F>
        struct count_if
        {
            typedef int type;
        };
    }

    template <typename Seq, typename F>
    inline int
    count_if(BOOST_FUSION_R_ELSE_LREF(Seq) seq, BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return fold(
            BOOST_FUSION_FORWARD(Seq,seq),
            0,
            detail::count_if_helper<BOOST_FUSION_R_ELSE_LREF(F)>(f));
    }
}}

#endif

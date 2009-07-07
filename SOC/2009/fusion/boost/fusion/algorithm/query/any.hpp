/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_ANY_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_ANY_HPP

#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename FRef>
        struct any_helper
        {
            typedef bool result_type;

            any_helper(FRef f)
              : f(f)
            {}

            template<typename E>
            inline bool
            operator()(BOOST_FUSION_R_ELSE_LREF(E) e)
            {
                return !f(BOOST_FUSION_FORWARD(E,e));
            }

            FRef f;
        };
    }

    namespace result_of
    {
        template <typename Seq, typename F>
        struct any
        {
            typedef bool type;
        };
    }

    template <typename Seq, typename F>
    inline bool
    any(BOOST_FUSION_R_ELSE_LREF(Seq) seq, BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return !all(
                BOOST_FUSION_FORWARD(Seq,seq),
                detail::any_helper<BOOST_FUSION_R_ELSE_LREF(F)>(f));
    }
}}

#endif

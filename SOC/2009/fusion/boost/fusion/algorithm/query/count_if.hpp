/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_COUNT_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_COUNT_IF_HPP

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

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
            operator()(BOOST_FUSION_R_ELSE_CLREF(E) e, int count)const
            {
                return f(BOOST_FUSION_FORWARD(E,e)) ? ++count : count;
            }

            FRef f;
        };
    }

    namespace result_of
    {
        template <typename Seq, typename>
        struct count_if
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef int type;
        };
    }

    template <typename Seq, typename F>
    inline typename
        result_of::count_if<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(F)
        >::type
    count_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_R_ELSE_CLREF(F) f)
    {
        return fold(BOOST_FUSION_FORWARD(Seq,seq),
            0,
            detail::count_if_helper<BOOST_FUSION_R_ELSE_CLREF(F)>(f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F>
    inline typename result_of::count_if<Seq&,F const&>::type
    count_if(Seq& seq, F const& f)
    {
        return fold(seq,
            0,
            detail::count_if_helper<F const&>(f));
    }
#endif
}}

#endif

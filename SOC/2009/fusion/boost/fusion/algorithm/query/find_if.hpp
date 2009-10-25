/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_IF_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/workaround.hpp>

#include <boost/mpl/quote.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pred>
        struct find_if
          : detail::static_find_if<
                typename begin<Seq>::type
              , typename end<Seq>::type
              , mpl::bind1<
                    typename mpl::lambda<Pred>::type
                  , mpl::bind1<mpl::quote1<value_of>,mpl::_1>
                >
            >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    template <typename Pred, typename Seq>
    inline typename
        result_of::find_if<BOOST_FUSION_R_ELSE_CLREF(Seq), Pred>::type
    find_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::find_if<
                BOOST_FUSION_R_ELSE_CLREF(Seq), Pred
            >::call(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Pred, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::find_if<,Seq,&,Pred>)
    find_if(Seq& seq)
    {
        return result_of::find_if<Seq&, Pred>::call(fusion::begin(seq));
    }
#endif
}}

#endif

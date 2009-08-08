/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_IF_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/quote.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/placeholders.hpp>
#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
#   include <boost/type_traits/is_const.hpp>
#   include <boost/utility/enable_if.hpp>
#endif

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Pred>
        struct find_if
        {
            typedef
                detail::static_find_if<
                    typename begin<Seq>::type
                  , typename end<Seq>::type
                  , mpl::bind1<
                        typename mpl::lambda<Pred>::type
                      , mpl::bind1<mpl::quote1<value_of>,mpl::_1>
                    >
                >
            gen;

            typedef typename gen::type type;
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
            >::gen::call(fusion::begin(seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Pred, typename Seq>
#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    inline typename
        lazy_disable_if<
            is_const<Seq>
          , result_of::find_if<Seq&, Pred>
        >::type const
#else
    inline typename result_of::find_if<Seq&, Pred>::type const
#endif
    find_if(Seq& seq)
    {
        return result_of::find_if<Seq&, Pred>::gen::call(fusion::begin(seq));
    }
#endif
}}

#endif

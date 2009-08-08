/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/value_of.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>
#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
#   include <boost/type_traits/is_const.hpp>
#   include <boost/utility/enable_if.hpp>
#endif

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct find
        {
            typedef
                detail::static_seq_find_if<
                    typename begin<Seq>::type
                  , typename end<Seq>::type
                  , is_same<value_of<mpl::_1>, T>
                >
            filter;

            typedef typename filter::type type;
        };
    }

    template <typename T, typename Seq>
    inline typename
        result_of::find<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::type const
    find(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::find<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , T
            >::filter::call(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename T, typename Seq>
#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    inline typename
        lazy_disable_if<
            is_const<Seq>
          , result_of::find<Seq&, T>
        >::type const
#else
    inline typename result_of::find<Seq&, T>::type const
#endif
    find(Seq& seq)
    {
        return result_of::find<Seq&, T>::filter::call(seq);
    }
#endif
}}

#endif

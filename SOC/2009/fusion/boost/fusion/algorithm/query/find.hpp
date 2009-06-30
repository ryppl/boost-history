/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/fusion/algorithm/query/detail/find_if.hpp>
#include <boost/fusion/algorithm/query/detail/assoc_find.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<
            typename Seq
          , typename T
          , bool is_associative_sequence=
                traits::is_associative<Seq>::value
        >
        struct find;

        template <typename Seq, typename T>
        struct find<Seq, T, false>
        {
            typedef
                detail::static_seq_find_if<
                    typename result_of::begin<Seq>::type
                  , typename result_of::end<Seq>::type
                  , is_same<mpl::_, T>
                >
            filter;

            typedef typename filter::type type;
        };

        template <typename Seq, typename T>
        struct find<Seq, T, true>
        {
            typedef detail::assoc_find<Seq, T> filter;
            typedef typename filter::type type;
        };
    }

    template <typename T, typename Seq>
    inline typename
        result_of::find<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::type const
    find(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        typedef typename
            result_of::find<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::filter
        gen;

        return gen::call(seq);
    }
}}

#endif

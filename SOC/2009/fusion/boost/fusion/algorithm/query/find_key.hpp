/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_KEY_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_KEY_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/key_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/workaround.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct find_key
          : detail::static_seq_find_if<
                typename begin<Seq>::type
              , typename end<Seq>::type
              , is_same<key_of<mpl::_1>, T>
            >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_associative<Seq>));
        };
    }

    template <typename T, typename Seq>
    inline typename
        result_of::find_key<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::type const
    find_key(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::find_key<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::call(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename T, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::find_key<,Seq,&, T>) const
    find_key(Seq& seq)
    {
        return result_of::find_key<Seq&, T>::call(seq);
    }
#endif
}}

#endif

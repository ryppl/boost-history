/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_FIND_KEY_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_FIND_KEY_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/key_of.hpp>
#include <boost/fusion/support/assert.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/workaround.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct find_key
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                detail::static_seq_find_if<
                    typename begin<Seq>::type
                  , typename end<Seq>::type
                  , is_same<key_of<mpl::_1>, T>
                >
            filter;

            typedef typename filter::type type;
        };
    }

    template <typename T, typename Seq>
    inline typename
        result_of::find_key<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::type const
    find_key(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::find_key<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , T
            >::filter::call(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename T, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::find_key<,Seq,&, T>) const
    find_key(Seq& seq)
    {
        return result_of::find_key<Seq&, T>::filter::call(seq);
    }
#endif
}}

#endif

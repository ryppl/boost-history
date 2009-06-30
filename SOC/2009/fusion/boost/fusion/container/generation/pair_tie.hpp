/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined (BOOST_FUSION_PAIR_TIE_20060812_2058)
#define BOOST_FUSION_PAIR_TIE_20060812_2058

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/as_fusion_element.hpp>

#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion {

    template<typename Key, typename T>
    struct pair;

    namespace result_of
    {
        template<typename Key, typename T>
        struct pair_tie
        {
            typedef
                fusion::pair<Key
                           , typename detail::add_lref<
                                 typename detail::as_fusion_element<T>::type
                             >::type
                >
            type;
        };
    }

    template<typename Key, typename T>
    typename result_of::pair_tie<Key, BOOST_FUSION_R_ELSE_LREF(T)>::type
    pair_tie(BOOST_FUSION_R_ELSE_LREF(T) t)
    {
        typedef typename
            result_of::pair_tie<Key, BOOST_FUSION_R_ELSE_LREF(Sequence)>::type
        type;
        return type(BOOST_FUSION_FORWARD(T,t));
    }

    template<typename Key, typename T>
    typename result_of::pair_tie<Key, T const&>::type
    pair_tie(T const& t)
    {
        return typename result_of::pair_tie<Key, T const&>::type(t);
    }
}}

#endif

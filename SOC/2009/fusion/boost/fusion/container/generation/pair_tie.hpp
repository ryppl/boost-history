/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined (BOOST_FUSION_PAIR_TIE_20060812_2058)
#define BOOST_FUSION_PAIR_TIE_20060812_2058

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    template<typename Key, typename T>
    struct pair;

    namespace result_of
    {
        template<typename Key, typename T>
        struct pair_tie
        {
            typedef
                fusion::pair<
                    Key
                  , typename detail::as_fusion_element_ref<T>::type
                >
            type;
        };
    }

    template<typename Key, typename T>
    typename result_of::pair_tie<Key, BOOST_FUSION_R_ELSE_CLREF(T)>::type
    pair_tie(BOOST_FUSION_R_ELSE_LREF(T) t)
    {
        return typename
            result_of::pair_tie<
                Key
              , BOOST_FUSION_R_ELSE_CLREF(T)
            >::type(BOOST_FUSION_FORWARD(T,t));
    }
}}

#endif

/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_JOIN_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_JOIN_HPP

#include <boost/fusion/view/joint_view.hpp>

namespace boost { namespace fusion {

    namespace result_of
    {
        template<typename Lhs, typename Rhs>
        struct join
        {
            typedef
                joint_view<
                    typename detail::add_lref<Lhs>::type
                  , typename detail::add_lref<Rhs>::type
                >
            type;
        };
    }

    template<typename Lhs, typename Rhs>
    inline typename
        result_of::join<
            BOOST_FUSION_R_ELSE_LREF(Lhs)
          , BOOST_FUSION_R_ELSE_LREF(Rhs)
        >::type
    join(BOOST_FUSION_R_ELSE_LREF(Lhs) lhs,
            BOOST_FUSION_R_ELSE_LREF(Rhs) rhs)
    {
        return typename
            result_of::join<
                BOOST_FUSION_R_ELSE_LREF(Lhs)
              , BOOST_FUSION_R_ELSE_LREF(Rhs)
            >::type(
                    BOOST_FUSION_FORWARD(Lhs,lhs)
                  , BOOST_FUSION_FORWARD(Rhs,rhs));
    }
}}

#endif

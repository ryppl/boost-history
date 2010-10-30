/*==============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_CONS_TIE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_CONS_TIE_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/support/deduce.hpp>

namespace boost { namespace fusion
{
    struct nil;

    namespace result_of
    {
        template<typename Car, typename Cdr = nil>
        struct cons_tie
        {
            typedef
                cons<
                    typename detail::deduce_ref<Car>::type
                  , typename traits::deduce<Cdr>::type
                >
            type;
        };
    }

    // $$$ do we really want a cons_tie? $$$
    template<typename Car>
    inline typename result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)>::type
    cons_tie(BOOST_FUSION_R_ELSE_LREF(Car) car)
    {
        return typename
            result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)>::type(
                BOOST_FUSION_FORWARD(Car,car));
    }

    template<typename Car, typename Cdr>
    inline typename
        result_of::cons_tie<
            BOOST_FUSION_R_ELSE_LREF(Car)
          , BOOST_FUSION_R_ELSE_LREF(Cdr)
        >::type
    cons_tie(BOOST_FUSION_R_ELSE_LREF(Car) car
           , BOOST_FUSION_R_ELSE_LREF(Cdr) cdr)
    {
        return typename
            result_of::cons_tie<
                BOOST_FUSION_R_ELSE_LREF(Car)
              , BOOST_FUSION_R_ELSE_LREF(Cdr)>::type(
                    BOOST_FUSION_FORWARD(Car,car)
                  , BOOST_FUSION_FORWARD(Cdr,cdr));
    }
}}

#endif

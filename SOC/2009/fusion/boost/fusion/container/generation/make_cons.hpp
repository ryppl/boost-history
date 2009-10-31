/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_MAKE_CONS_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_MAKE_CONS_HPP

#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/support/deduce.hpp>

namespace boost { namespace fusion
{
    struct nil;

    namespace result_of
    {
        template <typename Car, typename Cdr = nil>
        struct make_cons
        {
            typedef
                cons<
					typename traits::deduce<Car>::type
                 , typename traits::deduce<Cdr>::type
                >
            type;
        };
    }

    template <typename Car>
    inline typename result_of::make_cons<BOOST_FUSION_R_ELSE_CLREF(Car)>::type
    make_cons(BOOST_FUSION_R_ELSE_CLREF(Car) car)
    {
        typedef typename
            result_of::make_cons<BOOST_FUSION_R_ELSE_CLREF(Car)>::type
        type;

        return type(BOOST_FUSION_FORWARD(Car,car));
    }

    template <typename Car, typename Cdr>
    inline typename
        result_of::make_cons<
            BOOST_FUSION_R_ELSE_CLREF(Car)
          , BOOST_FUSION_R_ELSE_CLREF(Cdr)
        >::type
    make_cons(BOOST_FUSION_R_ELSE_CLREF(Car) car,
              BOOST_FUSION_R_ELSE_CLREF(Cdr) cdr)
    {
        typedef typename
            result_of::make_cons<
                BOOST_FUSION_R_ELSE_CLREF(Car)
              , BOOST_FUSION_R_ELSE_CLREF(Cdr)
            >::type
        type;

        return type(BOOST_FUSION_FORWARD(Car,car)
                  , BOOST_FUSION_FORWARD(Cdr,cdr));
    }

}}

#endif

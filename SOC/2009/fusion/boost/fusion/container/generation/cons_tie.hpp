/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_CONS_TIE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_CONS_TIE_HPP

//TODO cschmidt: ?!

#include <boost/fusion/container/list/cons.hpp>

namespace boost { namespace fusion
{
    struct nil;

    namespace result_of
    {
        template <typename Car, typename Cdr = nil>
        struct cons_tie
        {
            typedef
                cons<typename detail::add_lref<
                        typename detail::as_fusion_element<Car>::type
                     >::type
                   , typename detail::as_fusion_element<Cdr>::type>
            type;
        };
    }

    // $$$ do we really want a cons_tie? $$$
    template <typename Car>
    inline typename result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)>::type
    cons_tie(BOOST_FUSION_R_ELSE_LREF(Car) car)
    {
        typedef typename
            result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)>::type
        type;
        return type(BOOST_FUSION_FORWARD(Car,car));
    }

    template <typename Car, typename Cdr>
    inline typename result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)
                                      , BOOST_FUSION_R_ELSE_LREF(Cdr)>::type
    cons_tie(BOOST_FUSION_R_ELSE_LREF(Car) car
           , BOOST_FUSION_R_ELSE_LREF(Cdr) cdr)
    {
        typedef typename
            result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)
                              , BOOST_FUSION_R_ELSE_LREF(Cdr)>::type
        type;
        return type(BOOST_FUSION_FORWARD(Car,car)
                  , BOOST_FUSION_FORWARD(Cdr,cdr));
    }

    template <typename Car, typename Cdr>
    inline typename result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car)
                                      , Cdr const&>::type
    cons_tie(BOOST_FUSION_R_ELSE_LREF(Car) car, Cdr const& cdr)
    {
        typedef typename
            result_of::cons_tie<BOOST_FUSION_R_ELSE_LREF(Car), Cdr const&>::type
        type;
        return type(BOOST_FUSION_FORWARD(Car,car), cdr);
    }
}}

#endif

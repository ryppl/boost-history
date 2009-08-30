#ifndef BOOST_MPL_AUX_ADVANCE_BACKWARD_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_AUX_ADVANCE_BACKWARD_HPP_VARIADIC_TEMPLATES

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/evansl/prog_dev/boost-cvs/ro/boost/sandbox/variadic-templates/boost/mpl/aux_/RCS/advance_backward.hpp,v $
// $Date: 2007/03/05 23:45:03 $
// $Revision: 1.3 $
//ChangeLog:
//  2006-10-23.1334:Larry Evans
//    WHAT:
//      1) cp'ed from $BOOST_ROOT/boost/mpl/aux_/advance_backward.hpp
//      2) replaced all code within include guards with variadic
//         version of code in:
//           $BOOST_ROOT/boost/mpl/aux_/preprocessed/gcc/advance_backward.hpp
//    WHY:
//      1-2) avoid use of preprocessed advance_backward.hpp because
//         it uses apply_wrap1 which has been replaced with variadic template
//         version of apply_wrap, and there's no variadic template version
//         of apply_wrap1.

#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/prior.hpp>

namespace boost { namespace mpl { namespace aux {

template< BOOST_MPL_AUX_NTTP_DECL(long, N) > 
struct advance_backward
{
    static long const boost_mpl_limit_unrolling=1;
    template< typename Iterator > struct apply
    {
        typedef typename apply_wrap<
              advance_backward<boost_mpl_limit_unrolling>
            , Iterator
            >::type chunk_result_;

        typedef typename apply_wrap<
              advance_backward<(
                (N - boost_mpl_limit_unrolling) < 0
                    ? 0
                    : N - boost_mpl_limit_unrolling
                    )>
            , chunk_result_
            >::type type;
    };
};

template<>
struct advance_backward<0>
{
    template< typename Iterator > struct apply
    {
    //MAINTENANCE_NOTE:2006-10-23.1358
    //  The following (till };) was simply copied from 
    //  boost-root/boost/mpl/aux_/preprocessed/gcc/advance_backward.hpp
        typedef Iterator iter0;
        typedef iter0 type;
    };
};

template<>
struct advance_backward<1>
{
    template< typename Iterator > struct apply
    {
    //MAINTENANCE_NOTE:2006-10-23.1358
    //  The following (till };) was simply copied from 
    //  boost-root/boost/mpl/aux_/preprocessed/gcc/advance_backward.hpp
        typedef Iterator iter0;
        typedef typename prior<iter0>::type iter1;
        typedef iter1 type;
    };
};

}}}//exit boost::mpl::aux namespace

#endif // BOOST_MPL_AUX_ADVANCE_BACKWARD_HPP_VARIADIC_TEMPLATES

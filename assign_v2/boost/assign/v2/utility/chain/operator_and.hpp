//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2009 Neil Groves                                          //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_ALIAS_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_ALIAS_ER_2010_HPP
#include <boost/assign/v2/utility/chain/meta.hpp>
#include <boost/assign/v2/utility/chain/chain.hpp>

namespace boost{
namespace assign{
namespace v2{

#define BOOST_ASSIGN_V2_MACRO(U1, U2) \
template<typename R1, typename R2> \
typename chain_aux::result< \
    U1,\
    U2 \
>::type \
operator&&(U1& r1, U2 & r2) \
{ \
    return r1 | _chain( r2 ); \
} \
/**/
BOOST_ASSIGN_V2_MACRO( R1      , R2       )
BOOST_ASSIGN_V2_MACRO( R1      , R2 const )
BOOST_ASSIGN_V2_MACRO( R1 const, R2       )
BOOST_ASSIGN_V2_MACRO( R1 const, R2 const )
#undef BOOST_ASSIGN_V2_MACRO

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CHAIN_ALIAS_ER_2010_HPP
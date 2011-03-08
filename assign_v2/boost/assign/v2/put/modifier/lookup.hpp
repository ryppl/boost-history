//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_LOOKUP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_LOOKUP_ER_2010_HPP
#include <boost/assign/v2/put/modifier/lookup/tag.hpp>
#include <boost/assign/v2/put/modifier/lookup/impl.hpp>

#include <boost/assign/v2/put/modulo/modifier/keyword.hpp>
BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_KEYWORD(lookup)

#include <boost/assign/v2/put/modulo/modifier/meta.hpp>
#define BOOST_ASSIGN_V2_TAG modifier_tag::lookup<Arg>
BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_META_MODIFIER_TAG(lookup, BOOST_ASSIGN_V2_TAG)
#undef BOOST_ASSIGN_V2_TAG

#endif // BOOST_ASSIGN_V2_PUT_MODIFIER_LOOKUP_ER_2010_HPP


#ifndef BOOST_MPL_VECTOR_VECTOR0_HPP_INCLUDED
#define BOOST_MPL_VECTOR_VECTOR0_HPP_INCLUDED

// + file: boost/mpl/vector/vector0.hpp
// + last modified: 12/may/03

// Copyright (c) 2000-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/aux_/config/vector.hpp"

#if defined(BOOST_MPL_CFG_TYPEOF_BASED_VECTOR_IMPL)
#   define BOOST_MPL_AUX_VECTOR_IMPL_HEADER( header ) 


BOOST_PP_STRINGIZE(boost/mpl/vector/aux_/header)


#include "boost/mpl/vector/aux_/header.hpp"

#include BOOST_MPL_AUX_VECTOR_IMPL_HEADER(vector0.hpp)

/*
#include "boost/mpl/vector/aux_/at.hpp"
#include "boost/mpl/vector/aux_/front.hpp"
#include "boost/mpl/vector/aux_/push_front.hpp"
#include "boost/mpl/vector/aux_/pop_front.hpp"
#include "boost/mpl/vector/aux_/back.hpp"
#include "boost/mpl/vector/aux_/clear.hpp"
#include "boost/mpl/vector/aux_/O1_size.hpp"
#include "boost/mpl/vector/aux_/size.hpp"
#include "boost/mpl/vector/aux_/empty.hpp"
#include "boost/mpl/vector/aux_/item.hpp"
#include "boost/mpl/vector/aux_/iterator.hpp"
#include "boost/mpl/vector/aux_/begin_end.hpp"
*/
#include "boost/mpl/vector/aux_/vector0.hpp"
#include "boost/mpl/vector/aux_/tag.hpp"


#endif // BOOST_MPL_VECTOR_VECTOR0_HPP_INCLUDED

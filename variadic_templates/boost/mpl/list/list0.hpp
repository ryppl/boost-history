
#ifndef BOOST_MPL_LIST_LIST0_HPP_INCLUDED
#define BOOST_MPL_LIST_LIST0_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/05/08 17:09:17 $
// $Revision: 1.2 $

#include <boost/mpl/long.hpp>
#include <boost/mpl/list/aux_/push_front.hpp>
#include <boost/mpl/list/aux_/pop_front.hpp>
#include <boost/mpl/list/aux_/push_back.hpp>
#include <boost/mpl/list/aux_/front.hpp>
#include <boost/mpl/list/aux_/clear.hpp>
#include <boost/mpl/list/aux_/O1_size.hpp>
#include <boost/mpl/list/aux_/size.hpp>
#include <boost/mpl/list/aux_/empty.hpp>
#include <boost/mpl/list/aux_/begin_end.hpp>
#include <boost/mpl/list/aux_/item.hpp>
#include <boost/mpl/list/aux_/list0.hpp>
//|
//!@nv-mpl_diff
//!  The non-variadic mpl did not the above
//!  #include of aux_/list0.hpp.
//!  It also had a declaration of list0.
//!  This declaration has been moved to
//!  the aux_/list0.hpp file and renamed
//!  to list<>.
//!
//!  The rationale for this change is to make
//!  the list directory structure as close
//!  as possible to the vector directory structure.
//!  This should make understanding the directory
//!  structure for sequences easier.

#endif // BOOST_MPL_LIST_LIST0_HPP_INCLUDED

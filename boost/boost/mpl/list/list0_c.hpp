
#ifndef BOOST_MPL_LIST_LIST0_C_HPP_INCLUDED
#define BOOST_MPL_LIST_LIST0_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/list/list0.hpp>

namespace boost { namespace mpl {

template< typename T > struct list0_c
    : null_node
{
    typedef null_node type;
};

}}

#endif // BOOST_MPL_LIST_LIST0_C_HPP_INCLUDED


#ifndef BOOST_MPL_AUX_LARGEST_INT_HPP_INCLUDED
#define BOOST_MPL_AUX_LARGEST_INT_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

namespace boost { namespace mpl { namespace aux {

template< typename T1, typename T2 > struct largest_int
{
    typedef T1 type;
};

}}

#endif // BOOST_MPL_AUX_LARGEST_INT_HPP_INCLUDED

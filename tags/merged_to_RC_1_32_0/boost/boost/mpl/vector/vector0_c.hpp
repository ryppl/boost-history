
#ifndef BOOST_MPL_VECTOR_VECTOR0_C_HPP_INCLUDED
#define BOOST_MPL_VECTOR_VECTOR0_C_HPP_INCLUDED

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

#include <boost/mpl/vector/vector0.hpp>

namespace boost { namespace mpl {

template< typename T > struct vector0_c
    : vector0<>
{
    typedef vector0_c type;
};

}}

#endif // BOOST_MPL_VECTOR_VECTOR0_C_HPP_INCLUDED

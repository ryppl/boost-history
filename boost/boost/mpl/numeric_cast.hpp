
#ifndef BOOST_MPL_NUMERIC_CAST_HPP_INCLUDED
#define BOOST_MPL_NUMERIC_CAST_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

namespace boost { namespace mpl {

// no default implementation; the definition is needed to make MSVC happy

template< typename Tag1, typename Tag2 > struct numeric_cast
{
    template< typename N > struct apply;
};

}}

#endif // BOOST_MPL_NUMERIC_CAST_HPP_INCLUDED

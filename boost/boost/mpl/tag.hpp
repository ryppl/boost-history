
#ifndef BOOST_MPL_TAG_HPP_INCLUDED
#define BOOST_MPL_TAG_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2004
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

template< class T > struct tag
{
    typedef typename T::tag type;
};

}}

#endif // BOOST_MPL_TAG_HPP_INCLUDED

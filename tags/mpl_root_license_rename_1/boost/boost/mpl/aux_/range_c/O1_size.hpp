
#ifndef BOOST_MPL_AUX_RANGE_C_O1_SIZE_HPP_INCLUDED
#define BOOST_MPL_AUX_RANGE_C_O1_SIZE_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/O1_size_fwd.hpp>
#include <boost/mpl/aux_/range_c/size.hpp>
#include <boost/mpl/aux_/range_c/tag.hpp>

namespace boost { namespace mpl {

template<>
struct O1_size_impl< aux::half_open_range_tag >
    : size_impl< aux::half_open_range_tag >
{
};

}}

#endif // BOOST_MPL_AUX_RANGE_C_O1_SIZE_HPP_INCLUDED

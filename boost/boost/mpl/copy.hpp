//-----------------------------------------------------------------------------
// boost mpl/copy.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_COPY_HPP
#define BOOST_MPL_COPY_HPP

#include "boost/mpl/for_each.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<
      typename InputSequence
    , typename OutputSequence
    >
struct copy
{
    typedef typename mpl::for_each<
                  InputSequence
                , OutputSequence
                , mpl::push_back<mpl::_1,mpl::_2>
                >::state sequence;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COPY_HPP

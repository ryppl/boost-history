//-----------------------------------------------------------------------------
// boost mpl/transform.hpp header file
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

#ifndef BOOST_MPL_TRANSFORM_HPP
#define BOOST_MPL_TRANSFORM_HPP

#include "boost/mpl/for_each.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/compose/f_x_hy.hpp"
#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<
      typename InputSequence
    , typename OutputSequence
    , typename UnaryOperation
    >
struct transform
{
    typedef typename mpl::for_each<
          InputSequence
        , OutputSequence
        , mpl::compose_f_x_hy<
                mpl::push_back<mpl::placeholder,mpl::placeholder>
              , UnaryOperation
              >
        >::state sequence;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_TRANSFORM_HPP

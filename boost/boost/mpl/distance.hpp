//-----------------------------------------------------------------------------
// boost mpl/distance.hpp header file
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

#ifndef BOOST_MPL_DISTANCE_HPP
#define BOOST_MPL_DISTANCE_HPP

#include "boost/mpl/for_loop.hpp"
#include "boost/mpl/identity/project1st.hpp"
#include "boost/mpl/utility/next.hpp"
#include "boost/mpl/compose/f_gxy.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/same_as.hpp"
#include "boost/mpl/et.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<
      typename FirstIterator
    , typename LastIterator
    >
struct distance
{
 private:
    typedef mpl::for_loop< 
          FirstIterator
        , mpl::not_same_as<LastIterator>
        , mpl::next<mpl::_1>
        , mpl::int_t<0>
        , mpl::compose_f_gxy<
                mpl::next<mpl::placeholder>
              , mpl::project1st<mpl::_1,mpl::_2>
              >
        > loop_;

 public:
    typedef typename loop_::executed::state type;
    BOOST_STATIC_CONSTANT(long, value = type::value);
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_DISTANCE_HPP

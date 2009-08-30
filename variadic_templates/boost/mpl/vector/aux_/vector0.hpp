
#ifndef BOOST_MPL_VECTOR_AUX_VECTOR0_HPP_INCLUDED
#define BOOST_MPL_VECTOR_AUX_VECTOR0_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/05/09 19:46:56 $
// $Revision: 1.6 $

#include <boost/mpl/long.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/aux_/type_wrapper.hpp>

#include <boost/mpl/vector/aux_/tag.hpp>
#include <boost/mpl/aux_/config/typeof.hpp>

//!  
//!@nv-mpl_diff:
//!  WHAT:
//!    1) Add following #include.
//!    2) Rm'd 
//!       a) #else part of 
//!          #if defined(BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)
//!       b) #include interator.hpp 
//!  WHY:
//!    1) To allow the following vector<> specialization.
//!    2) a) Not needed since with gcc compiler,
//!          defined(BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)
//!       b) Only needed by #else part.
//!
#include <boost/mpl/vector_fwd.hpp>

namespace boost
{
namespace mpl
{

  template
  <
  >
  struct
vector
  <
  >
//!  
//!@nv-mpl_repl:
//!  This specialization replaces the vector0 template
//!  defined in non-variadic mpl file:
//!    boost/mpl/vector/aux_/vector0.hpp
//!
{
    typedef aux::vector_tag tag;
    typedef vector         type;
    static long const push_back0_index=
      0
    //|
    //!@nv-mpl_diff
    //!  The non-variadic mpl did not have this.
    //!  Instead, it just put this constant
    //!  in the following lower_bound_ typedef.
    //!
    //!  The reason for putting it here is to
    //!  document the purpose of this number.
    //!
    //!@change_log:2009-03-29.Larry Evans
    //!  Also, the value is different.
    //!  In the non-variadic mpl, this value
    //!  was 32768.  Since I see no reason
    //!  for this value, I just chose a less
    //!  "mysterious" value together with
    //!  a little more documentation in the
    //!  @brief below.
    //!
    
    /**@brief
     *  This is push_back<vector<>,T1>::type::index_::value
     *  for any typename, T1.
     *  See boost/mpl/vector/aux_/item.hpp 
     *  (at specialization for at_front==1)
     *  and boost/mpl/vector/aux_/push_front.hpp.
     *
     *  There's really not much special about the
     *  value chosen.  It would be most any value.
     *  However, if push_front is called enough times,
     *  the resulting type, B, could have an underflow
     *  in B::index_ due to lower bound on long values.
     *  Likewise, if push_back is called enough times,
     *  the resulting type, F, could have an overflow
     *  in F::index_ due to upper bound on long values.
     */
    ;
    typedef long_<push_back0_index> lower_bound_;
    typedef lower_bound_    upper_bound_;
    typedef long_<0>        size;
    
    static aux::type_wrapper<void_> item_(...);
};

}//exit mpl namespace
}//exit boost namespace


#endif // BOOST_MPL_VECTOR_AUX_VECTOR0_HPP_INCLUDED

//-----------------------------------------------------------------------------
// boost mpl/advance.hpp header file
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

#ifndef BOOST_MPL_ADVANCE_HPP
#define BOOST_MPL_ADVANCE_HPP

#include "boost/mpl/for_loop.hpp"
#include "boost/mpl/comparison/less.hpp"
#include "boost/mpl/math/abs.hpp"
#include "boost/mpl/identity/project1st.hpp"
#include "boost/mpl/compose/f_gxy.hpp"
#include "boost/mpl/utility/next.hpp"
#include "boost/mpl/utility/prior.hpp"
#include "boost/mpl/select_type.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<typename Iterator, long N>
struct advance
{
 private:
    typedef typename mpl::select_type<
          (N < 0)
        , mpl::prior<mpl::placeholder>
        , mpl::next<mpl::placeholder>
        >::type next_func_;
        
    typedef typename mpl::for_loop< 
          mpl::int_t<0>
        , mpl::lt<(mpl::abs<N>::value)>
        , next_func_
        , Iterator
        , mpl::compose_f_gxy<
                next_func_
              , mpl::project1st<mpl::_1,mpl::_2>
              >
        > loop_;

 public:
    typedef typename loop_::executed::state iterator;
};

template<typename Iterator, typename N>
struct advance_t
    : advance<Iterator, N::value>
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_ADVANCE_HPP

//-----------------------------------------------------------------------------
// boost mpl/for_each.hpp header file
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

#ifndef BOOST_MPL_FOR_EACH_HPP
#define BOOST_MPL_FOR_EACH_HPP

#include "boost/mpl/for_loop.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/utility/next.hpp"
#include "boost/mpl/utility/value_type.hpp"
#include "boost/mpl/compose/f_x_hy.hpp"
#include "boost/mpl/same_as.hpp"
#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<
      typename Sequence
    , typename State
    , typename BinaryFunction
    >
struct for_each
{
 private:
    typedef typename mpl::begin<Sequence>::iterator first_;
    typedef typename mpl::end<Sequence>::iterator last_;
    typedef mpl::for_loop< 
          first_
        , mpl::not_same_as<last_>
        , mpl::next<mpl::_1>
        , State
        , mpl::compose_f_x_hy< BinaryFunction,mpl::value_type<mpl::_1> >
        > loop_;
 
 public:
    typedef typename loop_::executed::state state;
    
    template<typename T>
    static void inline execute(T t)
    {
        loop_::execute(t);
    }

    static void inline execute()
    {
        loop_::execute();
    }
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FOR_EACH_HPP

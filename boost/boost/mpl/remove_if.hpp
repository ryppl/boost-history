//----------------------------------------------------------------------------
// boost mpl/remove_if.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//----------------------------------------------------------------------------
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

#ifndef BOOST_MPL_REMOVE_HPP
#define BOOST_MPL_REMOVE_HPP

#include "boost/mpl/copy_if.hpp"
#include "boost/mpl/clear.hpp"
#include "boost/mpl/logical/not.hpp"
#include "boost/mpl/compose/f_gx.hpp"
#include "boost/mpl/wrapper/unary_predicate.hpp"
#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<
      typename Sequence
    , typename Predicate
    >
struct remove_if
{
 private:
    typedef typename mpl::clear<Sequence>::sequence result_;
 
 public:
    typedef typename mpl::copy_if<
          Sequence
        , result_
        , mpl::compose_f_gx<
                mpl::logical_not<mpl::placeholder>
              , mpl::unary_predicate<Predicate>
              >
        >::sequence sequence;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_REMOVE_HPP

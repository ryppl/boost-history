//-----------------------------------------------------------------------------
// boost mpl/count_if.hpp header file
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

#ifndef BOOST_MPL_COUNT_IF_HPP
#define BOOST_MPL_COUNT_IF_HPP

#include "boost/mpl/for_each.hpp"
#include "boost/mpl/select_type.hpp"
#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<typename Predicate>
struct next_if
{
    template<typename N, typename U> struct apply
    {
        typedef typename mpl::select_type< 
              ::boost::mpl::unary_function<Predicate, U>::value
            , typename N::next
            , N
            >::type type;
    };
};

} // namespace detail

template<
      typename Sequence
    , typename Predicate
    >
struct count_if
{
 private:
    typedef mpl::for_each< 
          Sequence
        , mpl::int_t<0>
        , mpl::detail::next_if<Predicate>
        > for_each_;
 
 public:
    typedef typename for_each_::state type;
    BOOST_STATIC_CONSTANT(long, value = type::value);
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COUNT_IF_HPP

//-----------------------------------------------------------------------------
// boost mpl/copy_if.hpp header file
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

#ifndef BOOST_MPL_COPY_IF_HPP
#define BOOST_MPL_COPY_IF_HPP

#include "boost/mpl/for_each.hpp"
#include "boost/mpl/select_type.hpp"
#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/faked_typedef.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<typename Predicate>
struct push_back_if
{
    template<typename Sequence, typename U> struct apply
    {
        typedef typename mpl::select_type<
              ::boost::mpl::unary_function<Predicate, U>::value
            , mpl::push_back<Sequence, U>
            , mpl::detail::faked_typedef_sequence<Sequence>
            >::type::sequence type;
    };
};

} // namespace detail

template<
      typename InputSequence
    , typename OutputSequence
    , typename Predicate
    >
struct copy_if
{
    typedef typename mpl::for_each< 
          InputSequence
        , OutputSequence
        , mpl::detail::push_back_if<Predicate>
        >::state sequence;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_COPY_IF_HPP

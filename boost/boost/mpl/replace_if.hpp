//-----------------------------------------------------------------------------
// boost mpl/replace_if.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy and John R. Bandela
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_REPLACE_IF_HPP
#define BOOST_MPL_REPLACE_IF_HPP

#include "boost/mpl/transform.hpp"
#include "boost/mpl/clear.hpp"
#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/select_type.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<typename Predicate, typename NewValue>
struct replace_helper
{
    template<typename U> struct apply
    {
        typedef typename mpl::select_type<
              mpl::unary_function<Predicate,U>::value
            , NewValue
            , U
            >::type type;
    };
};

} // namespace detail

template<
      typename Sequence
    , typename Predicate
    , typename NewValue
    >
struct replace_if
{
 private:
    typedef typename mpl::clear<Sequence>::sequence result_;
    typedef mpl::detail::replace_helper<Predicate,NewValue> replacer_;

 public:
    typedef typename mpl::transform<
          Sequence
        , result_
        , replacer_
        >::sequence sequence;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_REPLACE_IF_HPP

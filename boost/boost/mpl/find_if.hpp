//-----------------------------------------------------------------------------
// boost mpl/find_if.hpp header file
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

#ifndef BOOST_MPL_FIND_IF_HPP
#define BOOST_MPL_FIND_IF_HPP

#include "boost/mpl/while_true.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/utility/next.hpp" 
#include "boost/mpl/utility/value_type.hpp"
#include "boost/mpl/logical/or.hpp"
#include "boost/mpl/compose/f_gx.hpp"
#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/et.hpp"
#include "boost/type_traits/same_traits.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<
      typename LastIterator
    , typename Predicate
    >
struct find_if_predicate
{
    template<typename Iterator> struct apply
    {
     private:
        typedef boost::is_same<Iterator,LastIterator> end_of_sequence_;
        typedef mpl::compose_f_gx< Predicate,mpl::value_type<mpl::placeholder> > pred_;
        typedef mpl::unary_function<pred_,Iterator> type_found_;

     public:
        BOOST_STATIC_CONSTANT(bool, value = 
            !(mpl::logical_or<end_of_sequence_, type_found_>::value)
            );
    };
};

} // namespace detail


template<typename Sequence, typename Predicate>
struct find_if
{
 private:
    typedef typename mpl::begin<Sequence>::iterator first_;
    typedef typename mpl::end<Sequence>::iterator last_;
    typedef mpl::while_true<
          mpl::detail::find_if_predicate<last_, Predicate>
        , first_
        , mpl::next<mpl::_1>
        > loop_;

 public:
    typedef typename loop_::executed::state iterator;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FIND_IF_HPP

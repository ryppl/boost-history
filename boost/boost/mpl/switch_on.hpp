//-----------------------------------------------------------------------------
// boost mpl/switch_on.hpp header file
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

#ifndef BOOST_MPL_SWITCH_ON_HPP
#define BOOST_MPL_SWITCH_ON_HPP

#include "boost/mpl/type_list.hpp"
#include "boost/mpl/find_if.hpp" 
#include "boost/mpl/identity/identity.hpp" 
#include "boost/mpl/comparison/equal_to.hpp" 
#include "boost/mpl/same_as.hpp" 
#include "boost/mpl/select_type.hpp" 
#include "boost/mpl/int_t.hpp" 
#include "boost/mpl/faked_typedef.hpp" 
#include "boost/mpl/preprocessor/default_template_params.hpp" 
#include "boost/type_traits/same_traits.hpp" 

namespace boost {
namespace mpl {

namespace detail {

struct switch_case_default;

template<typename T>
struct case_find_predicate
{
    template<typename U> struct apply
    {
        BOOST_STATIC_CONSTANT(bool, value = 
            (::boost::mpl::unary_function<typename U::compare, T>::value)
            );
    };
};

template<typename T, typename CaseList>
struct switch_impl
{
 private:
    typedef case_find_predicate<T> case_pred_;
    typedef case_find_predicate<switch_case_default> default_pred_;
    
    typedef typename mpl::find_if<CaseList,case_pred_>::iterator iter_;
    typedef typename mpl::end<CaseList>::iterator last_;
 
 public:
    typedef typename mpl::select_type< 
          ::boost::is_same<iter_,last_>::value
        , mpl::find_if<CaseList, default_pred_>
        , mpl::detail::faked_typedef_iterator<iter_>
        >::type::iterator::type::result type;
};
    
} // namespace detail

template<typename Key>
struct type_case
{
    template<typename T> struct type 
    {
        typedef mpl::same_as<Key> compare;
        typedef mpl::identity<T> result;
    };

    template<long N> struct value
    {
        typedef mpl::same_as<Key> compare;
        typedef mpl::int_t<N> result;
    };
};

template<long M>
struct value_case
{
    template<typename T> struct type
    {
        typedef mpl::eq<M> compare;
        typedef mpl::identity<T> result;
    };
        
    template<long N> struct value
    {
        typedef mpl::eq<M> compare;
        typedef mpl::int_t<N> result;
    };
};

struct default_case
    : type_case<mpl::detail::switch_case_default>
{
};

template<
      typename T
    , BOOST_MPL_DEFAULT_TEMPLATE_PARAMS(typename C, null_argument)
    > 
struct switch_on
    : mpl::detail::switch_impl<
            T
          , mpl::type_list<BOOST_MPL_TEMPLATE_PARAMS(C)>
          >::type
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_SWITCH_ON_HPP

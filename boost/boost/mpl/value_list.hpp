//-----------------------------------------------------------------------------
// boost mpl/value_list.hpp header file
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

#ifndef BOOST_MPL_VALUE_LIST_HPP
#define BOOST_MPL_VALUE_LIST_HPP

#include "boost/mpl/list/preprocessor.hpp"
#include "boost/mpl/list/traits.hpp"
#include "boost/mpl/list/factory.hpp"
#include "boost/mpl/list/algorithm.hpp"
#include "boost/mpl/null.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

struct value_list_tag;

template<>
struct list_traits<value_list_tag>
    : mpl::basic_list_traits<value_list_tag>
{
    typedef mpl::null_t null_argument;
    
    template<typename List>
    struct value_type_
    {
        typedef typename List::value_type type;
        BOOST_STATIC_CONSTANT(long, value = type::value);
    };
};

#define BOOST_MPL_INT_VALUE_TEMPLATE_PARAMETER(i, param) \
    BOOST_PREPROCESSOR_COMMA_IF(i) mpl::int_t<param##i> \
/**/

#define BOOST_MPL_ENUMERATE_INT_VALUE_PARAMS(param) \
    BOOST_PREPROCESSOR_REPEAT( \
        BOOST_MPL_LIST_PARAMETERS_NUMBER \
      , BOOST_MPL_INT_VALUE_TEMPLATE_PARAMETER \
      , param \
      ) \
/**/

template< BOOST_MPL_LIST_DEFAULT_PARAMETERS(long N, mpl::null_t::value) >
struct value_list 
    : mpl::list_factory<
            value_list_tag
          , BOOST_MPL_ENUMERATE_INT_VALUE_PARAMS(N)
          >::type
{
    typedef typename mpl::list_factory<
            value_list_tag
          , BOOST_MPL_ENUMERATE_INT_VALUE_PARAMS(N)
          >::type type;
};

#undef BOOST_MPL_INT_VALUE_TEMPLATE_PARAMETER
#undef BOOST_MPL_ENUMERATE_INT_VALUE_PARAMS

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_VALUE_LIST_HPP

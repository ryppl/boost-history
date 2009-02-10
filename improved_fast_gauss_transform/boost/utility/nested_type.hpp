///////////////////////////////////////////////////////////////////////////////
// nested_type.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_UTILITY_NESTED_TYPE_HPP_ER_2009
#define BOOST_UTILITY_NESTED_TYPE_HPP_ER_2009
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost{namespace utility{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type);

// See news://news.gmane.org:119/gma8hc$2a0$1@ger.gmane.org
template<class T>
struct nested_type :
 boost::mpl::eval_if<has_type<T>,
   T,
   boost::mpl::identity<T> >
{};

}}
#endif

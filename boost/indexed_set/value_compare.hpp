/* Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_VALUE_COMPARE_HPP
#define BOOST_INDEXED_SET_VALUE_COMPARE_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/call_traits.hpp>
#include <functional>

namespace boost{

namespace indexed_sets{

namespace detail{

template<typename Value,typename KeyFromValue,typename Compare>
struct value_comparison:std::binary_function<Value,Value,bool>
{
  value_comparison(Compare comp_=Compare()):comp(comp_){}

  bool operator()(
    typename call_traits<Value>::param_type x,
    typename call_traits<Value>::param_type y)
  {
    return comp(key_fom_value(x),key_fom_value(y));
  }

private:
  Compare      comp;
  KeyFromValue key_fom_value;
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif

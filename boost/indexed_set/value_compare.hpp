/* Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
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
  value_comparison(KeyFromValue key_=KeyFromValue(),Compare comp_=Compare()):
    key(key_),comp(comp_)
  {
  }

  bool operator()(
    typename call_traits<Value>::param_type x,
    typename call_traits<Value>::param_type y)
  {
    return comp(key(x),key(y));
  }

private:
  KeyFromValue key;
  Compare      comp;
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif

//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_SWAP_CONTEXT_HPP_20060611
#define BOOST_COROUTINE_SWAP_CONTEXT_HPP_20060611
namespace boost{ namespace coroutines { namespace detail {
  class default_hint {};
  class yield_hint: public default_hint {};
  class yield_to_hint: public default_hint {};
  class invoke_hint: public default_hint {};
  
} } }
#endif

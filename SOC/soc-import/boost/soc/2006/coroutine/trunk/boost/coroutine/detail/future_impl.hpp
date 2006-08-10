//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060809
#define BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060809
#include <boost/optional.hpp>
#include <boost/assert.hpp>
namespace boost { namespace coroutines { namespace detail {
  template<typename TupleType>
  struct future_impl   {
      typedef TupleType tuple_type;
    future_impl() : 
      pending(false),
      waited(false)  {}
    bool pending;
    bool waited;
    boost::optional<tuple_type> optional;
    
    void assign(const tuple_type& val) {
      BOOST_ASSERT(pending);
      pending = false;
      optional = val;
    }

    void mark_pending() {
      pending = true;
    }

  };
} } }
#endif

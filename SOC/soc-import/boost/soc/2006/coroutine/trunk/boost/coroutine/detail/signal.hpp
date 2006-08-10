//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
#include <boost/coroutine/detail/future_impl.hpp>
namespace boost { namespace coroutines { namespace detail {
  /*
   * Private interface for coroutine signaling/waiting.
   * These class is friend of future_impl and
   * its static members are invoked by asynchronous callback functions.
   */
  struct wait_gateway {
    template<typename Future>
    static 
    bool waited(const Future& f) {
      return f.waited();
    }

    template<typename Future>
    static 
    void mark_wait(Future& f, bool how) {
      f.mark_wait(how);
    }

    template<typename Future>
    static
    BOOST_DEDUCED_TYPENAME 
    Future::impl_pointer
    get_impl(Future& f) {
      return f.m_ptr;
    }
  };


} } }
#endif

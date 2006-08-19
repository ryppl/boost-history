//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060809
#define BOOST_COROUTINE_DETAIL_FUTURE_IMPL_HPP_20060809
#include <boost/optional.hpp>
#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/coroutine/detail/coroutine_accessor.hpp>
#include <boost/coroutine/detail/context_base.hpp>

namespace boost { namespace coroutines { namespace detail {


  template<typename ValueType, typename ContextImpl>
  class future_impl : boost::noncopyable  {
  public:
    typedef ValueType value_type;
    typedef 
    context_base<ContextImpl> *      
    context_weak_pointer;
    
    typedef 
    BOOST_DEDUCED_TYPENAME
    context_base<ContextImpl>::pointer
    context_pointer;

    typedef boost::optional<value_type> pointer;

    template<typename CoroutineSelf>
    future_impl(CoroutineSelf& self) :
      m_coro_impl_weak(coroutine_accessor::get_impl(self)),
      m_coro_impl(0),
      m_waited(false)
    {}

    value_type& 
    value() {
      return *m_optional;
    }

    value_type const& 
    value() const{
      return *m_optional;
    }

    pointer& 
    get() {
      return m_optional;
    }

    pointer const& 
    get() const{
      return m_optional;
    }

    bool pending() {
      return 0 != m_coro_impl.get();
    }
    
    template<typename T>
    void assign(const T& val) {
      BOOST_ASSERT(pending());
      context_pointer p = m_coro_impl;
      m_coro_impl = 0;
      m_optional = val;
      p->count_down();
      if(waited() && p->signal())
	p->wake_up();
    }

    void mark_pending() {
      m_coro_impl = m_coro_impl_weak;
      m_coro_impl ->count_up();
    }

    void mark_wait(bool how) {
      m_waited = how;
    }

    bool waited() const {
      return m_waited;
    }

    context_pointer context() {
      BOOST_ASSERT(pending());
      return m_coro_impl;
    }
        
    void wait(int n) {
      m_coro_impl_weak->wait(n);
    }

    void wait() {
      if(!pending()) return;
      mark_wait(true);
      try {
	m_coro_impl->wait(1);
	BOOST_ASSERT(!pending());
      } catch (...) {
	mark_wait(false);
	throw;
      }
      mark_wait(false);
    }

  private:
    context_weak_pointer m_coro_impl_weak;
    context_pointer m_coro_impl;
    bool m_waited;
    boost::optional<value_type> m_optional;
  };
} } }
#endif

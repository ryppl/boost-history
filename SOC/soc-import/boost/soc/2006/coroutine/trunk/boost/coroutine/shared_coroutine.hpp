//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_SHARED_COROUTINE_HPP_20060812
#define BOOST_COROUTINE_SHARED_COROUTINE_HPP_20060812
#include <boost/coroutine/coroutine.hpp>
namespace boost { namespace coroutines {
  // This class is a workaround for the widespread lack of move
  // semantics support. It is a refrence counted wrapper around 
  // the coroutine object.
  // FIXME: ATM a shared_coroutine is-a coroutine. This is to avoid
  // inheriting privately and cluttering the code with lots of using
  // declarations to unhide coroutine members and nested types.
  // From a purity point of view, coroutines and shared_coroutines should
  // be two different types.
  template<typename Signature, typename ContextImpl = detail::default_context_impl> 
  class shared_coroutine : public coroutine<Signature, ContextImpl> {
  public:
    typedef coroutine<Signature, ContextImpl> coroutine_type;

    shared_coroutine() {}

    template<typename Functor>
    shared_coroutine(Functor f, 
		     std::ptrdiff_t stack_size = 
		     detail::default_stack_size) :
      coroutine_type(f, stack_size) {}

    shared_coroutine(move_from<coroutine_type> src):
      coroutine_type(src) {}

    shared_coroutine(const shared_coroutine& rhs) :
      coroutine_type(rhs.m_pimpl.get(), detail::init_from_impl_tag()) {}

    shared_coroutine& operator=(move_from<coroutine_type> src) {
      shared_coroutine(src).swap(*this);
      return *this;
    }

    shared_coroutine& operator=(const shared_coroutine& rhs) {
      shared_coroutine(rhs).swap(*this);
      return *this;
    }    
  private:
  };
} }
#endif

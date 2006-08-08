//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_COROUTINE_ACCESSOR_HPP_20060709
#define BOOST_COROUTINE_DETAIL_COROUTINE_ACCESSOR_HPP_20060709
namespace boost { namespace coroutines { namespace detail {
  
  /*
   * This is a private interface used for coroutine
   * implementation.
   */
  struct init_from_impl_tag{};
  struct coroutine_accessor {

    // Initialize coroutine from implementation type.
    template<typename Coroutine, typename Ctx>
    static
    Coroutine construct(Ctx * src) {
      return Coroutine(src, init_from_impl_tag());
    }

    template<typename Coroutine>
    static 
    void acquire(Coroutine& x) {
      x.acquire();
    }

    template<typename Coroutine>
    static
    void release(Coroutine& x) {
      x.release();
    }
  };
} } }
#endif

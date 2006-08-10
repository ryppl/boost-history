//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_COROUTINE_ACCESSOR_HPP_20060709
#define BOOST_COROUTINE_DETAIL_COROUTINE_ACCESSOR_HPP_20060709
#include  <boost/utility/in_place_factory.hpp>
namespace boost { namespace coroutines { namespace detail {
  
  /*
   * This is a private interface used for coroutine
   * implementation.
   */
  struct init_from_impl_tag{};
  struct coroutine_accessor {

    // Initialize coroutine from implementation type.
    // used by the in_place_assing in place factory.
    template<typename Coroutine, typename Ctx>
    static
    void construct(Ctx * src, void * address) {
      new (address) Coroutine(src, init_from_impl_tag());
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

    template<typename Ctx>
    struct in_place_assign : boost::in_place_factory_base {

      in_place_assign(Ctx * ctx) :
	m_ctx(ctx) {}

      template<typename Coroutine>
      void apply(void * memory) const {
	construct<Coroutine>(m_ctx, memory);
      }
      Ctx * m_ctx;
    };

    template<typename Ctx>
    static 
    in_place_assign<Ctx>
    in_place(Ctx * ctx) {
      return in_place_assign<Ctx>(ctx);
    }

    template<typename Coroutine>
    static
    typename Coroutine::impl_ptr
    get_impl(Coroutine& x) {
      return x.get_impl();
    }

    template<typename Coroutine>
    static
    typename Coroutine::impl_type *
    pilfer_impl(Coroutine& x) {
      return x.pilfer_impl();
    }

    template<typename Coroutine>
    static
    std::size_t
    count(const Coroutine& x) {
      return x.count();
    }
  };
} } }
#endif

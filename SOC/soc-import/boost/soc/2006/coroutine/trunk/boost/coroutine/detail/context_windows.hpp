//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_CONTEXT_WINDOWS_HPP_20060625
#define BOOST_COROUTINE_CONTEXT_WINDOWS_HPP_20060625
#include <windows.h>
#include <winnt.h>
#include <boost/config.hpp>
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/coroutine/exception.hpp>
#include <boost/coroutine/detail/swap_context.hpp>
namespace boost {namespace coroutines {namespace detail{
  namespace windows {

    typedef	LPVOID fiber_ptr;

    /*
     * This number (0x1E00) has been sighted in the wild (at least on windows XP systems)
     * as return value from GetCurrentFiber() on non fibrous threads. This is sowehow related
     * to OS/2 where the current fiber pointer is overloaded as a version field.
     * On non-NT systems, 0 is returned. 
     */
    fiber_ptr fiber_magic = reinterpret_cast<fiber_ptr>(0x1E00);
		
    /*
     * Return true if current thread is a fiber.
     * FIXME: on longhorn shoud use IsThreadAFiber
     */
    bool is_fiber() {
      fiber_ptr current = GetCurrentFiber();
      return current != 0 && current != fiber_magic;
    }

    fiber_ptr get_current_fiber() {

      if(!is_fiber()) {
	fiber_ptr current = ConvertThreadToFiber(0);
	BOOST_ASSERT(current != 0);
	(void)current;
      } 
      BOOST_ASSERT(is_fiber());
      return GetCurrentFiber();
    }
    /*
     * Posix implementation for the context_impl_base class.
     * @note context_impl is not required to be consistent
     * If not initialized it can only be swapped out, not in 
     * (at that point it will be initialized).
     *
     */
    class fibers_context_impl_base {
    public:
      /**
       * Create an empty context. 
       * An empty context cannot be restored from,
       * but can be saved in.
       */
      fibers_context_impl_base() :
	m_fixup(is_fiber()),
	m_ctx(get_current_fiber()) {}
	
      /*
       * Free function. Saves the current context in @p from
       * and restores the context in @p to. On windows the from
       * parameter is ignored. The current context is saved on the 
       * current fiber.
       */     

	
      friend 
      void 
      swap_context(fibers_context_impl_base&, 
		   const fibers_context_impl_base& to,
		   default_hint) {
	SwitchToFiber(to.m_ctx); 
      }

      ~fibers_context_impl_base() {
	if(m_fixup){
	  bool result = ConvertFiberToThread();
	  BOOST_ASSERT(result);
	  (void)result;
	}
      }
    protected:
      explicit
      fibers_context_impl_base(fiber_ptr ctx) :
	m_fixup(false),
	m_ctx(ctx) {}
      bool m_fixup;
      fiber_ptr m_ctx;
    };

    template<typename T>
    inline
    void
    trampoline() {
      T* fun = reinterpret_cast<T*>(GetFiberData());
      BOOST_ASSERT(fun);
      (*fun)();
    }

    class fibers_context_impl :
      public fibers_context_impl_base,
      private boost::noncopyable {
    public:
      typedef fibers_context_impl_base context_impl_base;

      enum {default_stack_size = 8192};

      /**
       * Create a context that on restore inovkes Functor on
       *  a new stack. The stack size can be optionally specified.
       */
      template<typename Functor>
      explicit
      fibers_context_impl(Functor& cb, std::ptrdiff_t stack_size) :
	fibers_context_impl_base
      (CreateFiber(stack_size== -1? 0 : stack_size,
		   reinterpret_cast<LPFIBER_START_ROUTINE>(&trampoline<Functor>),
		   reinterpret_cast<LPVOID>(&cb)))
      {
	BOOST_ASSERT(m_ctx);
      }
		  
      ~fibers_context_impl() {
	DeleteFiber(m_ctx);
      }
		  
    private:
    };

    typedef fibers_context_impl context_impl;

  }
} } }
#endif

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

    /*
     * Windows implementation for the context_impl_base class.
     * @note context_impl is not required to be consistent
     * If not initialized it can only be swapped out, not in 
     * (at that point it will be initialized).
     */
    class fibers_context_impl_base {
    public:
      /**
       * Create an empty context. 
       * An empty context cannot be restored from,
       * but can be saved in.
       */
      fibers_context_impl_base() :
	m_ctx(0) {}
	
      /*
       * Free function. Saves the current context in @p from
       * and restores the context in @p to. On windows the from
       * parameter is ignored. The current context is saved on the 
       * current fiber.
       * Note that if the current thread is not a fiber, it will be
       * converted to fiber on the fly on call and unconverted before
       * return. This is expensive. The user should convert the 
       * current thread to a fiber once on thread creation for better performance.
       * Note that we can't leave the thread unconverted on return or else we 
       * will leak resources on thread destruction. Do the right thing by
       * default.
       */     
      friend 
      void 
      swap_context(fibers_context_impl_base& from, 
		   const fibers_context_impl_base& to,
		   default_hint) {
	if(!is_fiber()) {
	  BOOST_ASSERT(from.m_ctx == 0);
	  from.m_ctx = ConvertThreadToFiber(0);
	  BOOST_ASSERT(from.m_ctx != 0);
	  
	  SwitchToFiber(to.m_ctx); 
	  
	  bool result = ConvertFiberToThread();
	  BOOST_ASSERT(result);
	  (void)result;
	  from.m_ctx = 0;
	} else {
	  bool call_from_main = from.m_ctx == 0;
	  if(call_from_main)
	    from.m_ctx = GetCurrentFiber();
	  SwitchToFiber(to.m_ctx); 
	  if(call_from_main)
	    from.m_ctx = 0;
	}
      }

      ~fibers_context_impl_base() {}
    protected:
      explicit
      fibers_context_impl_base(fiber_ptr ctx) :
	m_ctx(ctx) {}

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

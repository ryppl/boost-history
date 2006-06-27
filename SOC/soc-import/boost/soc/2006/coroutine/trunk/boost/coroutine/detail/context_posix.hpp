//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_CONTEXT_POSIX_HPP_20060601
#define BOOST_COROUTINE_CONTEXT_POSIX_HPP_20060601
#include <boost/config.hpp>

#if defined(_XOPEN_UNIX) && defined(_XOPEN_VERSION) && _XOPEN_VERSION >= 500

/*
 * makecontex based context implementation. Should be available on all
 * SuSv2 compliant UNIX systems. 
 * NOTE: this implementation is not
 * optimal as the makecontext API saves and restore the signal mask.
 * This requires a system call for every context switch that really kills
 * performance. Still is very portable and guaranteed to work.
 * NOTE2: makecontext and friends are declared obsolescent in SuSv3, but
 * it is unlikely that they will be removed any time soon.
 */
#include <ucontext.h>
#include <boost/noncopyable.hpp>
#include <boost/coroutine/exception.hpp>
#include <boost/coroutine/detail/posix_utility.hpp>
#include <boost/coroutine/detail/swap_context.hpp>
namespace boost { namespace coroutines { namespace detail {
  namespace posix {


    /*
     * Posix implementation for the context_impl_base class.
     * @note context_impl is not required to be consistent
     * If not initialized it can only be swapped out, not in 
     * (at that point it will be initialized).
     *
     */
    class ucontext_context_impl_base {
      /*
       * Free function. Saves the current context in @p from
       * and restores the context in @p to.
       */     
      friend 
      void 
      swap_context(ucontext_context_impl_base& from, 
		   const ucontext_context_impl_base& to,
		   default_hint) {
	int  error = ::swapcontext(&from.m_ctx, &to.m_ctx); 
	if(error != 0) {
	  throw swap_error();
	}
      }

    protected:
      ::ucontext_t m_ctx;
    };

    class ucontext_context_impl :
      public ucontext_context_impl_base,
      private boost::noncopyable {
    public:
      typedef ucontext_context_impl_base context_impl_base;

      enum {default_stack_size = 8192};

    
      /**
       * Create a context that on restore inovkes Functor on
       *  a new stack. The stack size can be optionally specified.
       */
      template<typename Functor>
      explicit
	ucontext_context_impl(Functor& cb, std::ptrdiff_t stack_size) :
      m_stack(alloc_stack(stack_size == -1? default_stack_size: stack_size)) {
	stack_size = stack_size == -1? default_stack_size: stack_size;
	int error = ::getcontext(&m_ctx);
	BOOST_ASSERT(error == 0);
	(void)error;
	m_ctx.uc_stack.ss_sp = m_stack;
	m_ctx.uc_stack.ss_size = stack_size;
	BOOST_ASSERT(m_stack);
	m_ctx.uc_link = 0;
	typedef void cb_type(Functor*);
	typedef void (*ctx_main)();
	cb_type * cb_ptr = &trampoline<Functor>; 

	//makecontext can't fail.
	::makecontext(&m_ctx,
		      (ctx_main)(cb_ptr), 
		      1,
		      &cb);
      }
      
      ~ucontext_context_impl() {
	if(m_stack)
	  free_stack(m_stack, m_ctx.uc_stack.ss_size);
      }

    private:
      void * m_stack;
    };

    typedef ucontext_context_impl context_impl;
  }
} } }
#else 

/**
 * This is just a temporary placeholder. Context swapping can
 * be implemented on all most posix systems lacking *context using the
 * sigaltstack+longjmp trick.
 * Eventually it will be implemented, but for now just throw an error.
 * Most posix systems are at least SuSv2 compliant anyway.
 */
#error No context implementation for this POSIX system.

#endif
#endif

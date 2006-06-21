//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_CONTEXT_LINUX_HPP_20060601
#define BOOST_COROUTINE_CONTEXT_LINUX_HPP_20060601

#if defined(__GNUC__) && defined(__i386__) && !defined(BOOST_COROUTINE_NO_ASM)
#include <cstdlib>
#include <boost/coroutine/detail/posix_utility.hpp>
#include <boost/coroutine/detail/swap_context.hpp>

/*
 * Defining BOOST_COROUTINE_INLINE_ASM will enable the inlin3
 * assembler verwsion of swapcontext_stack.
 * The inline asm, with all required clobber flags, is no faster
 * than the out-of-line function, but is quite unsafe in the face of
 * optimizations.
 */
//#define BOOST_COROUTINE_INLINE_ASM

/* 
 * Defining BOOST_COROUTINE_SEPARATE_CALL_SITE will enable separate 
 * invoke, yield and yield_to swap_context functions. These can give on P4 
 * a 50% performance increase at the cost of a slightly higher
 * instruction cache use.
 */
#define BOOST_COROUTINE_SEPARATE_CALL_SITE


#ifndef BOOST_COROUTINE_INLINE_ASM
extern "C" void swapcontext_stack (void***, void**) throw()  __attribute((regparm(2)));
extern "C" void swapcontext_stack2 (void***, void**) throw()  __attribute((regparm(2)));
extern "C" void swapcontext_stack3 (void***, void**) throw()  __attribute((regparm(2)));
#else

void 
inline
swapcontext_stack(void***from_sp, void**to_sp) throw() {
  asm volatile
    ("\n\t pushl %%ebp"
     "\n\t pushl %[from]"
     "\n\t pushl %[to]"
     "\n\t pushl $0f"
     "\n\t movl %%esp, (%[from])" 
     "\n\t movl %[to], %%esp"
     "\n\t popl %%ecx" 
     "\n\t jmp  *%%ecx"
     "\n0:\t popl %[to]"
     "\n\t popl %[from]"
     "\n\t popl %%ebp"
     :: 
     [from] "a" (from_sp),
     [to]   "d" (to_sp)
     :
     "cc", 
     "%ecx",
     "%ebx", 
     "%edi", 
     "%esi",
     "%st",
     "%st(1)",
     "%st(2)",
     "%st(3)",
     "%st(4)",
     "%st(5)",
     "%st(6)",
     "%st(7)",
     "memory"
   );
  
}

void 
inline
swapcontext_stack2(void***from_sp, void**to_sp) throw() {
  swapcontext_stack(from_sp, to_sp);
}


#endif
namespace boost { namespace coroutines { namespace detail {
  namespace oslinux {
    template<typename T>
    void trampoline(T* fun);
    
    template<typename T>
    inline
    void
    trampoline(T * fun) { //fun in %eax
      (*fun)();
      std::abort();
    }

    class ia32_gcc_context_impl_base {
    public:
      ia32_gcc_context_impl_base() {};

      /**
       * Free function. Saves the current context in @p from
       * and restores the context in @p to.
       * @note This function is found by ADL.
       */     
      friend 
      void 
      swap_context(ia32_gcc_context_impl_base& from, 
		   ia32_gcc_context_impl_base const& to, 
		   default_hint) {
	swapcontext_stack(&from.m_sp, to.m_sp);
      }

#ifdef BOOST_COROUTINE_SEPARATE_CALL_SITE
      friend 
      void 
      swap_context(ia32_gcc_context_impl_base& from, 
		   ia32_gcc_context_impl_base const& to,
		   yield_hint) {
	swapcontext_stack2(&from.m_sp, to.m_sp);
      }

      friend 
      void 
      swap_context(ia32_gcc_context_impl_base& from, 
		   ia32_gcc_context_impl_base const& to,
		   yield_to_hint) {
	swapcontext_stack2(&from.m_sp, to.m_sp);
      }

#endif

    protected:
      void ** m_sp;
 
    };

    class ia32_gcc_context_impl  : public ia32_gcc_context_impl_base{
    public:
      enum {default_stack_size = 8192};
      
      typedef ia32_gcc_context_impl_base context_impl_base;

      ia32_gcc_context_impl() :
	m_stack(0) {}
      /**
       * Create a context that on restore invokes Functor on
       *  a new stack. The stack size can be optionally specified.
       */
      template<typename Functor>
      ia32_gcc_context_impl(Functor& cb, ssize_t stack_size = -1) :
	m_stack_size(stack_size == -1? default_stack_size: stack_size),
	m_stack(posix::alloc_stack(m_stack_size)) {
	m_sp = ((void**)m_stack + m_stack_size),
	BOOST_ASSERT(m_stack);
	typedef void fun(Functor*);// __attribute((noreturn, regparm(1)));
	fun * funp = trampoline;
#ifndef BOOST_COROUTINE_INLINE_ASM
	*--m_sp = &cb;     // parm 0 of trampoline;
	*--m_sp = 0;        // dummy return address for trampoline
	*--m_sp = (void*) funp ;// return addr (here: start addr)  NOTE: the unsafe cast is safe on IA32
	*--m_sp = 0;       // ebp                                  
	*--m_sp = 0;       // ebx                                  
	*--m_sp = 0;       // esi                                  
	*--m_sp = 0;       // edi        
#else
	*--m_sp = &cb;     // parm 0 of trampoline;
	*--m_sp = 0;        // dummy return address for trampoline
	*--m_sp = (void*) funp ;// return addr (here: start addr)  NOTE: the unsafe cast is safe on IA32
#endif
      }
      
      ~ia32_gcc_context_impl() {
	if(m_stack)
	  posix::free_stack(m_stack, m_stack_size);
      }

    private:
      ssize_t m_stack_size;
      void * m_stack;
    };
    
    typedef ia32_gcc_context_impl context_impl;
  }
} } }

#elif defined(__linux)
/**
 * For all other linux systems use the standard posix implementation.
 */
#include <boost/coroutine/detail/context_posix.hpp>
namespace boost { namespace coroutines { namespace detail { namespace oslinux {
    typedef posix::context_impl context_impl;
} } } }
#else
#error This header can only be included when compiling for linux systems.
#endif


#endif

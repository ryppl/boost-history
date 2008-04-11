//  Copyright (c) 2006, Giovanni P. Deretta
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINE_CONTEXT_LINUX_HPP_20060601
#define BOOST_COROUTINE_CONTEXT_LINUX_HPP_20060601

#if defined(__GNUC__) && defined(__i386__) && !defined(BOOST_COROUTINE_NO_ASM)
#include <cstdlib>
#include <cstddef>
#include <boost/assert.hpp>
#include "posix_utility.hpp"
#include "context_interface.hpp"


/**
 * Defining BOOST_COROUTINE_NO_SEPARATE_CALL_SITES will disable separate 
 * invoke, yield and yield_to swap_context functions. Separate calls sites
 * increase performance by 25% at least on P4 for invoke+yield back loops
 * at the cost of a slightly higher instruction cache use and is thus enabled by
 * default.
 */
//#define BOOST_COROUTINE_NO_SEPARATE_CALL_SITES

// FIXE: we are putting these in the global namespace! should mangle
// them with a boost_coroutine_prefix
extern "C" void swapcontext_stack (void***, void**) throw()  __attribute((regparm(2)));
extern "C" void swapcontext_stack2 (void***, void**) throw()  __attribute((regparm(2)));
extern "C" void swapcontext_stack3 (void***, void**) throw()  __attribute((regparm(2)));

namespace boost { namespace coroutines { namespace detail {
  namespace oslinux {

    struct  ia32_gcc_context_interface {

      enum { default_stack_size = 4096 * 10 };

      struct context_storage {
        void ** m_sp;    // %esp
        void * m_stack;  // stack buffer
        std::ptrdiff_t m_stack_size;
      };
        
      /**
       * Free function. Saves the current context in @p from
       * and restores the context in @p to.
       */     
      static 
      void 
      swap_context(context_storage& from, 
                   context_storage const& to, 
                   hints::unspecified) {
        swapcontext_stack(&from.m_sp, to.m_sp);
      }

#ifndef BOOST_COROUTINE_NO_SEPARATE_CALL_SITES
      static
      void 
      swap_context(context_storage& from,
                   context_storage const& to,
                   hints::yield) {
        swapcontext_stack2(&from.m_sp, to.m_sp);
      }

      static
      void 
      swap_context(context_storage& from, 
                   context_storage const& to,
                   hints::yield_to) {
        swapcontext_stack2(&from.m_sp, to.m_sp);
      }

#endif

      /**
       * Create a context that on restore invokes Functor on a new
       * stack. The stack size can be optionally specified.  This
       * should better be implemented in assembler, for better
       * robustness, but this should work anyways.
       */
      static void
      make_context
      (  context_storage& buffer, 
         callback_type& cb,
         void * state, 
         std::ptrdiff_t stack_size ) {
        buffer.m_stack = posix::alloc_stack(stack_size);
        BOOST_ASSERT(buffer.m_stack);
        buffer.m_stack_size = stack_size;
        buffer.m_sp = ((void**)buffer.m_stack + (buffer.m_stack_size/sizeof(void*)));

        *--buffer.m_sp = state;      // parm 0 of 'cb', i.e. state.
        *--buffer.m_sp = 0;           // dummy return address for cb
        *--buffer.m_sp = (void*) cb ;// return addr (here: start addr)  NOTE: the unsafe cast is safe on IA32
        *--buffer.m_sp = 0;           // ebp                                  
        *--buffer.m_sp = 0;           // ebx                                  
        *--buffer.m_sp = 0;           // esi                                  
        *--buffer.m_sp = 0;           // edi        
      }
      
      static void 
      destroy_context (context_storage&buffer){
        BOOST_ASSERT(buffer.m_stack);
        posix::free_stack(buffer.m_stack, buffer.m_stack_size);
      }
      
    };
  }}}}

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

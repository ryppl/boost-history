// Copyright (c) 2006, Giovanni P. Deretta
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_CONTEXT_INTERFACE_20080321
# define BOOST_COROUTINE_DETAIL_CONTEXT_INTERFACE_20080321
namespace boost{ namespace coroutines { 

  namespace hints {
    class unspecified {};
    class yield: public unspecified {};
    class yield_to : public unspecified {};
    class invoke : public unspecified {};
  }

namespace detail {
  extern "C" {
    typedef void (*callback_type)(void*);
  }

  /// @internal
  /// Interface to system specific context implementation.
  ///
  /// This class is only used as documentation for the ContextInterface
  /// concept. Actual implementations are system specific.
  struct context_interface_archetype {
    
    /// Implementation specific context storage.
    /// Models @c POD (often a plain pointer).
    ///
    /// A default initialized @p context_storage logically points to
    /// undefined state.  A call to make_context will initialize its
    /// argument to captured state. A call to @c swap_context can be
    /// used to capture the current state. A @p context_storage
    /// inizialized with @c make_context must be explicitly destroyed
    /// with a call to @c destroy_context.
    ///
    /// @note <tt>context_storage</tt>s can be freely copied, but only
    /// the last of such copies can be used (i.e. it has logical move
    /// semantics)
    struct unspecified {};
    typedef unspecified context_storage;

    static const size_t default_stack_size;

    /// Make a new context that will call <tt> cb(state)</tt> on start-up.
    ///
    /// @param to where the context state will be saved.
    ///
    /// @param cb pointer to a function to be bound to the context.
    /// 
    /// @param state parameter to be passed to @p cb.
    ///
    /// @param stack_size size of the stack for the new context.
    ///
    /// @post @p to is initialized with a valid context. When the
    /// context is activated by @c swap_context for the first time, @c
    /// cb(state) is invoked. The context owns its stack and other
    /// context specific data which must be freed by a call to @c
    /// destroy_context.
    /// 
    /// @note @c cb must never return. Instead @c exit_context
    /// should be used to tear down a context.
    static
    void make_context(    
      context_storage& to,
      callback_type cb,
      void* state,
      size_t stack_size
    );

    /// Capture the current context and restore a previously captured
    /// one.
    ///
    /// @param save where to save the current context.
    ///
    /// @param restore context to be resumed.
    ///
    /// @param hint this is one of <tt> hints::yield, hints::yield_to,
    /// hints::invoke </tt>.
    /// 
    /// @pre @c restore contains context information initialized by a
    /// previous call to @c swap_context or @c make_context. The
    /// context (or any of its copies) have never been restored before
    /// by a call to @c swap_context.
    ///
    /// @post @c save is initialized with the current contest
    /// state. Then the context state contained in @c restore becomes
    /// the current context. @c restore can no longer be restored from
    /// until is initialized again with a call to @c make_context or
    /// @c swap_context.
    ///
    /// @note this function might actually be overloaded by the
    /// different hints for optimization purposes, but all overloads
    /// should have the same behavior.
    static 
    void swap_context(context_storage& save, 
                      context_storage&restore, 
                      hints::unspecified hint);

    /// Destroy a context.  
    ///
    /// @param ctx a reference to a context initially created by a call
    /// to @c make_context.
    ///
    /// @post free long term context information (for example the
    /// context stack) associated with this context_storage.
    ///
    /// @pre this context has been initialized with a call to @c
    /// make_context. The context has not been destroyed yet.
    static 
    void destroy_context(context_storage& ctx) throw();
    
  };


}}}
#endif

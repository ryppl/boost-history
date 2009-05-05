namespace boost {
  namespace detail {
    namespace function {
      template<
         typename FunctionPtr,
         typename R, 
         typename ... Args
         >
      struct function_invoker
      {
         static R invoke(function_buffer& function_ptr, Args... args)
         {
           FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
           return f(args...);
         }
      };

      template<
         typename FunctionPtr,
         typename R,
         typename ... Args
         >
      struct void_function_invoker
      {
         static void
         invoke(function_buffer& function_ptr, Args... args)

         {
           FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
           BOOST_FUNCTION_RETURN(f(args...));
         }
      };

      template<
         typename FunctionObj,
         typename R,
         typename ... Args
      >
      struct function_obj_invoker
      {
         static R invoke(function_buffer& function_obj_ptr, Args... args)

         {
           FunctionObj* f;
           if (function_allows_small_object_optimization<FunctionObj>::value)
             f = reinterpret_cast<FunctionObj*>(&function_obj_ptr.data);
           else
             f = reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
           return (*f)(args...);
         }
      };

      template<
         typename FunctionObj,
         typename R,
         typename ... Args
      >
      struct void_function_obj_invoker
      {
         static void
         invoke(function_buffer& function_obj_ptr, Args... args)

         {
           FunctionObj* f;
           if (function_allows_small_object_optimization<FunctionObj>::value)
             f = reinterpret_cast<FunctionObj*>(&function_obj_ptr.data);
           else
             f = reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
           BOOST_FUNCTION_RETURN((*f)(args...));
         }
      };

      template<
         typename FunctionObj,
         typename R,
         typename ... Args
      >
      struct function_ref_invoker
      {
         static R invoke(function_buffer& function_obj_ptr, Args... args)

         {
           FunctionObj* f = 
             reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
           return (*f)(args...);
         }
      };

      template<
         typename FunctionObj,
         typename R,
         typename ... Args
      >
      struct void_function_ref_invoker
      {
         static void
         invoke(function_buffer& function_obj_ptr, Args... args)

         {
           FunctionObj* f = 
             reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
           BOOST_FUNCTION_RETURN((*f)(args...));
         }
      };

      /* Handle invocation of member pointers. */
      template<
         typename MemberPtr,
         typename R,
         typename ... Args
      >
      struct function_mem_invoker
      {
         static R invoke(function_buffer& function_obj_ptr, Args... args)

         {
           MemberPtr* f = 
             reinterpret_cast<MemberPtr*>(&function_obj_ptr.data);
           return boost::mem_fn(*f)(args...);
         }
      };

      template<
         typename MemberPtr,
         typename R,
         typename ... Args
      >
      struct function_void_mem_invoker
      {
         static void
         invoke(function_buffer& function_obj_ptr, Args... args)

         {
           MemberPtr* f = 
             reinterpret_cast<MemberPtr*>(&function_obj_ptr.data);
           BOOST_FUNCTION_RETURN(boost::mem_fn(*f)(args...));
         }
      };

      template<
         typename FunctionPtr,
         typename R,
         typename ... Args
      >
      struct get_function_invoker
      {
         typedef typename mpl::if_c<(is_void<R>::value),
                                void_function_invoker<
                                FunctionPtr,
                                R,
                                Args...
                             >,
                             function_invoker<
                                FunctionPtr,
                                R,
                                Args...
                             >
                          >::type type;
      };

      template<
         typename FunctionObj,
         typename R,
         typename ... Args
        >
      struct get_function_obj_invoker
      {
         typedef typename mpl::if_c<(is_void<R>::value),
                                void_function_obj_invoker<
                                FunctionObj,
                                R,
                                Args...
                             >,
                             function_obj_invoker<
                                FunctionObj,
                                R,
                                Args...
                             >
                          >::type type;
      };

      template<
         typename FunctionObj,
         typename R,
         typename ... Args
        >
      struct get_function_ref_invoker
      {
         typedef typename mpl::if_c<(is_void<R>::value),
                                void_function_ref_invoker<
                                FunctionObj,
                                R,
                                Args...
                             >,
                             function_ref_invoker<
                                FunctionObj,
                                R,
                                Args...
                             >
                          >::type type;
      };

      /* Retrieve the appropriate invoker for a member pointer.  */
      template<
         typename MemberPtr,
         typename R,
         typename ... Args
        >
      struct get_member_invoker
      {
         typedef typename mpl::if_c<(is_void<R>::value),
                                function_void_mem_invoker<
                                MemberPtr,
                                R,
                                Args...
                             >,
                             function_mem_invoker<
                                MemberPtr,
                                R,
                                Args...
                             >
                          >::type type;
      };

      /* Given the tag returned by get_function_tag, retrieve the
          actual invoker that will be used for the given function
          object. 

          Each specialization contains an "apply" nested class template
          that accepts the function object, return type, function
          argument types, and allocator. The resulting "apply" class
          contains two typedefs, "invoker_type" and "manager_type",
          which correspond to the invoker and manager types. */
      template<typename Tag>
      struct get_invoker { };

      /* Retrieve the invoker for a function pointer. */
      template<>
      struct get_invoker<function_ptr_tag>
      {
         template<typename FunctionPtr,
                   typename R, typename ... Args>
         struct apply
         {
           typedef typename get_function_invoker<
                                 FunctionPtr,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef functor_manager<FunctionPtr> manager_type;
         };

         template<typename FunctionPtr,
                   typename Allocator,
                   typename R, typename ... Args>
         struct apply_a
         {
           typedef typename get_function_invoker<
                                 FunctionPtr,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef functor_manager<FunctionPtr> manager_type;
         };
      };

      /* Retrieve the invoker for a member pointer. */
      template<>
      struct get_invoker<member_ptr_tag>
      {
         template<typename MemberPtr,
                   typename R, typename ... Args>
         struct apply
         {
           typedef typename get_member_invoker<
                                 MemberPtr,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef functor_manager<MemberPtr> manager_type;
         };

         template<typename MemberPtr,
                   typename Allocator,
                   typename R, typename ... Args>
         struct apply_a
         {
           typedef typename get_member_invoker<
                                 MemberPtr,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef functor_manager<MemberPtr> manager_type;
         };
      };

      /* Retrieve the invoker for a function object. */
      template<>
      struct get_invoker<function_obj_tag>
      {
         template<typename FunctionObj,
                   typename R, typename ... Args>
         struct apply
         {
           typedef typename get_function_obj_invoker<
                                 FunctionObj,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef functor_manager<FunctionObj> manager_type;
         };

         template<typename FunctionObj,
                   typename Allocator,
                   typename R, typename ... Args>
         struct apply_a
         {
           typedef typename get_function_obj_invoker<
                                 FunctionObj,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef functor_manager_a<FunctionObj, Allocator> manager_type;
         };
      };

      /* Retrieve the invoker for a reference to a function object. */
      template<>
      struct get_invoker<function_obj_ref_tag>
      {
         template<typename RefWrapper,
                   typename R, typename ... Args>
         struct apply
         {
           typedef typename get_function_ref_invoker<
                                 typename RefWrapper::type,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef reference_manager<typename RefWrapper::type> manager_type;
         };

         template<typename RefWrapper,
                   typename Allocator,
                   typename R, typename ... Args>
         struct apply_a
         {
           typedef typename get_function_ref_invoker<
                                 typename RefWrapper::type,
                                 R,
                                 Args...
                              >::type
             invoker_type;

           typedef reference_manager<typename RefWrapper::type> manager_type;
         };
      };
    } // end namespace function
  } // end namespace detail
} // end namespaces boost


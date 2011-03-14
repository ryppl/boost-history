//Prototype varadic template bind, bindv.
//====================================================================
//  (C) Copyright Larry Evans 2011.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
//#define USE_VARIADIC_BIND
#ifdef USE_VARIADIC_BIND
  #include <boost/mpl/package_c.hpp>
  #include <boost/type_traits/remove_cv.hpp>
  #include <boost/bind/placeholders.hpp>
  #include <boost/composite_storage/pack/multiple_dispatch/apply_unpack.hpp>
#else
  #include <boost/phoenix/bind.hpp>
  #include <boost/phoenix/core/argument.hpp>
#endif
#include <iostream>

namespace boost
{
namespace sandbox
{
#ifdef USE_VARIADIC_BIND
  template
  < unsigned Size
  >
  struct 
void_ptr_size_array
  {
          static
        unsigned const
      size
        =Size
        ;
          typedef
        void*
      ptr_type
        ;
        ptr_type
      my_ptrs[size]
        ;
        
        void*
      ptr_at(unsigned index)const
      {
          return my_ptrs[index];
      }
      
        void*&
      ptr_at(unsigned index)
      {
          return my_ptrs[index];
      }
      
    //XTORS:
      void_ptr_size_array
        ( void
        )
        {
          #ifdef VOID_PTR_ARRAY_SIZES_INITIALIZE
            //This is not needed for a correct program.
            //However, if, somehow, the my_ptr's are
            //not initialized somewhere else (for example
            //by replace_source_with_target_ptr), then
            //detecting whether they are properly intialized
            //becomes difficult because, without the
            //following initialization loop, the values
            //stored in my_ptrs, maybe non-zero and therefore 
            //difficult to check for correctness.
            for(unsigned i=0; i<size; ++i) my_ptrs[i]=0;
          #endif
        }
        
        template
        < typename... Types
        >
      void_ptr_size_array
        ( Types*... ptrs
        )
        : my_ptrs({ptrs...})
        {}
  };

  template
  < typename Types
  >
  struct 
void_ptr_array
  ;
  
  template
  < typename Type
  >
  typename remove_cv<Type>::type*
remove_cv_ptr
  (   Type& 
    value
  )
  /**@brief
   *  Removes 'const and volatile' from Type and returns
   *  pointer to value with that type. 
   */
  {
      typedef typename remove_cv<Type>::type remove_cv_type;
      remove_cv_type*rm_cv_ptr=const_cast<remove_cv_type*>(&value);
      return rm_cv_ptr;
  }
  
  template
  < typename... Types
  >
  struct 
void_ptr_array
  < mpl::package
    < Types...
    >
  >
  /**@brief
   *  1) An array of void* to Types...*
   *  2) A CTOR which takes Types&.. to create these pointers.
   *  3) A project<I>() function to convert 
   *     these pointers back to the Types&... passed to CTOR.
   */
  : void_ptr_size_array
    < sizeof...(Types)
    >
  {
          typedef
        void_ptr_size_array
        < sizeof...(Types)
        >
      super_type
        ;
          typedef typename
        super_type::ptr_type
      ptr_type
        ;
          using
        super_type::
      ptr_at
        ;
        template
        < unsigned Index
        >
        struct 
      result_type
        : mpl::at_c
          < mpl::package<Types...>
          , Index
          >
        {
        };
        
        template
        < unsigned Index
        >
        typename result_type<Index>::type&
      project(void)const
        {
              ptr_type 
            vp=ptr_at(Index);
          #ifdef VOID_PTR_ARRAY_SIZES_INITIALIZE
            assert(vp != 0);
          #endif
                typedef
              typename result_type<Index>::type
            arg_type;
            arg_type*ap=static_cast<arg_type*>(vp);
            return *ap;
        }
      
    //XTORS:
      void_ptr_array()
        {}
      void_ptr_array(Types&... args)
        : super_type
          ( remove_cv_ptr(args)...
          )
        {}
        
  };
  
  template
  < typename TargetFront
  , typename TargetBack
  , typename Source
  >
  struct
substitute
  /**@brief
   *  Subsittute values in TargetBack
   *  with values in Source. 
   */
;
  template
  < typename... Front
  , int Index
  , typename... Back
  , typename Source
  >
  struct
substitute
  < mpl::package<Front...>
  , mpl::package<arg<Index>,Back...>
  , Source
  >
{
          static
        unsigned const
      arg_i=Index-1
      ;
          typedef typename
        Source::template result_type<arg_i>::type
      arg_t
      ;
          typedef
        substitute
        < mpl::package<Front...,arg_t>
        , mpl::package<Back...>
        , Source
        >
      next_t
      /**@brief
       *   Substitute arg<Index> with the Index-th value in Source.
       */
      ;
          static
        auto
     _( void_ptr_size_array<sizeof...(Front)+1+sizeof...(Back)>& target
      , Source const& source
      )-> decltype(next_t::_(target,source))
      {
          unsigned const tar_index=sizeof...(Front);
          target.ptr_at(tar_index) = source.ptr_at(arg_i);
          return next_t::_(target,source);
      }
};

  template
  < typename... Front
  , typename    Middle
  , typename... Back
  , typename Source
  >
  struct
substitute
  < mpl::package<Front...>
  , mpl::package<Middle,Back...>
  , Source
  >
{
          typedef
        substitute
        < mpl::package<Front...,Middle>
        , mpl::package<Back...>
        , Source
        >
      next_t
      /**@brief
       *   Since Middle is not a placeholder, just move
       *   it to Front.
       */
      ;
          static
        auto
     _( void_ptr_size_array<sizeof...(Front)+1+sizeof...(Back)>& target
      , Source const& source
      )->decltype(next_t::_(target,source))
      {
          return next_t::_(target,source);
      }
};

  template
  < typename... Front
  , typename Source
  >
  struct
substitute
  < mpl::package<Front...>
  , mpl::package<>
  , Source
  >
{
          typedef
        void_ptr_array
        < mpl::package<Front...>
        >
      next_t
      ;
          static
        next_t&
     _( void_ptr_size_array<sizeof...(Front)>& target
      , Source const&
      )
      /**@brief
       *  Since no more arguments to be sustituted,
       *  just cast target to the actual type.
       */
      {
          return static_cast<next_t&>(target);
      }
};

  template
  < typename Functor
  , typename... ArgsPart
  >
  struct
bind_result
  : void_ptr_array
    < mpl::package<ArgsPart...>
    >
  {
          typedef
        void_ptr_array
        < mpl::package<ArgsPart...>
        >
      super_t;
        Functor
      my_ftor;
      bind_result
        ( Functor a_ftor
        , ArgsPart&... a_args
        )
      : super_t
        ( a_args...
        )
      , my_ftor(a_ftor)
      {} 
      
      template
      < typename... ArgsComplete
      >
      typename Functor::result_type
    operator()
      ( ArgsComplete&... a_args
      )
      {
          typedef mpl::package<ArgsComplete...> pkg_args;
          typedef void_ptr_array<pkg_args> source_t;
          source_t source_v(a_args...);
          super_t target_part(*this);
            auto 
          target_complete
            =substitute
             < mpl::package<>
             , mpl::package<ArgsPart...>
             , source_t
             >::_
             ( target_part
             , source_v
             );
             composite_storage::pack::multiple_dispatch::
            apply_unpack
            < typename mpl::package_range_c
              < unsigned
              , 0
              , sizeof...(ArgsPart)
              >::type
            >
          uapp;
          return uapp(my_ftor,target_complete);
      }
  };

  template
  < typename Functor
  , typename... Args
  >
  bind_result
  < Functor
  , Args...
  >
bind
  ( Functor a_ftor
  , Args&... a_args
  )
  {
    bind_result<Functor,Args...> bres(a_ftor,a_args...);
    return bres;
  }
  
  void
pkg_print
  (
  )
  {}
  
  template
  < typename Head
  , typename... Tail
  >
  void
pkg_print
  ( Head head
  , Tail... tail
  )
  {
    std::cout<<":"<<head;
    pkg_print(tail...);
  }
  
  struct
functor_any
  {
          typedef 
        void 
      result_type;
        template
        < typename... Args
        >
        result_type
      operator()
        ( Args&... a_args
        )
      {
        std::cout<<"functor_any(";
        pkg_print(a_args...);
        std::cout<<")\n";
      }
  };
#else
using namespace phoenix;
using namespace phoenix::placeholders;
  struct
functor_any
  {
          typedef 
        void 
      result_type;
        template
        < typename Arg1
        , typename Arg2
        >
        result_type
      operator()
        ( Arg1& a_arg1
        , Arg2& a_arg2
        )
      {
        std::cout<<"functor_any(";
        std::cout<<":"<<a_arg1;
        std::cout<<":"<<a_arg2;
        std::cout<<")\n";
      }
  };
#endif
  
  void
test()
  {
    int act1=1;
    int act2=2;
    functor_any f_any;
    {
      std::cout<<"(_1,_2)("<<act1<<","<<act2<<")\n";
      auto bnd_any=bind(f_any,_1,_2);
      bnd_any(act1,act2);
    }
    {
      std::cout<<"("<<act2<<",_1)("<<act1<<")\n";
      auto bnd_any=bind(f_any,act2,_1);
      bnd_any(act1);
      std::cout<<"("<<act2<<",_1)("<<act2<<")\n";
      bnd_any(act2);
    }
    {
      std::cout<<"("<<act2<<","<<act1<<")("<<act1<<")\n";
      auto bnd_any=bind(f_any,act2,act1);
      bnd_any(act1);
    }
  }  

}//exit sandbox namespace
}//exit boost namespace

int main()
{
  #ifdef USE_VARIADIC_BIND
  std::cout<<"YES_VARIADIC_BIND\n";
  #else
  std::cout<<"NOT_VARIADIC_BIND\n";
  #endif
  boost::sandbox::test();
  return 0;
}  

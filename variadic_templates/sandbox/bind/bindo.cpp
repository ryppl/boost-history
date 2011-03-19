//Prototype varadic template bind which using "overriding" to substitute values.
//====================================================================
//  (C) Copyright Larry Evans 2011.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
  #include <boost/mpl/package_c.hpp>
  #include <boost/type_traits/remove_cv.hpp>
  #include <boost/bind/placeholders.hpp>
  #include <boost/composite_storage/pack/multiple_dispatch/apply_unpack.hpp>
  #include <boost/type_traits/detail/wrap.hpp>
#include <iostream>

namespace boost
{
namespace sandbox
{
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
  < typename Target
  , typename Source
  >
  struct
substitute
  /**@brief
   *  Subsittute values in Target
   *  with values in Source. 
   *  Produce type suitable for use in
   *  apply_unpack template.
   */
;
  template
  < typename... Target
  , typename... Source
  >
  struct
substitute
  < void_ptr_array
    < mpl::package<Target...>
    >
  , void_ptr_array
    < mpl::package<Source...>
    >
  >
{
          typedef
        void_ptr_array
        < mpl::package<Target...>
        >
      target_t
      ;
          typedef
        void_ptr_array
        < mpl::package<Source...>
        >
      source_t
      ;
        target_t const&
      my_target
      ;
        source_t const
      my_source
      ;
        template
        < unsigned CompleteIndex
        , typename TargetType
        >
        struct 
      result_if_type
        /**@brief
         *  Returns type depending of
         *  the form of TargetType
         *  if TargetType is arg<SouceIndex>, for
         *    some sourceIndex, 
         *  then 
         *     returns SourceIndex-th Source...
         *  otherwise, 
         *     returns CompleteIndex-th Target...
         */
        ;

        template
        < unsigned CompleteIndex
        , typename TargetType//== CompleteIndex-th Target...
        >
        struct 
      result_if_type
        {
                typedef 
              TargetType 
            type
              ;
                static
              type&
            at
              ( substitute const& subs_this
              )
              {
                return subs_this.my_target.project<CompleteIndex>();
              }
        };

        template
        < unsigned CompleteIndex
        , int SourceIndex
        >
        struct 
      result_if_type
        < CompleteIndex
        , arg<SourceIndex> //TargetType == CompleteIndex-th Target...
                           //TargetType is placeholder, which must be
                           //"overrided" with SourceIndex-th source value.
        >
        {
                static
              unsigned const
            source_index
              =unsigned(SourceIndex-1)//arg indices start at 1 so decrement 
                                      //to create index starting at 0.
              ;
                typedef typename
              source_t::template result_type
              < source_index
              >::type
            type
              ;
                static
              type&
            at
              ( substitute const& subs_this
              )
              {
                return subs_this.my_source.project<source_index>();
              }
            
        };
        
        template
        < unsigned Index
        >
        struct 
      result_type
        : result_if_type
          < Index
          , typename mpl::at_c
            < mpl::package<Target...>
            , Index
            >::type
          >
        {
        };
        
        template
        < unsigned Index
        >
        typename result_type<Index>::type&
      project(void)const
        {
                typedef
              result_type<Index>
            res_t;
            return res_t::at(*this);
        }
        
      substitute
        ( target_t const& a_target
        , Source&... a_source
        )
        : my_target(a_target)
        , my_source(a_source...)
        {} 
        
};

  template
  < typename Functor
  , typename... ArgsPart
  >
  struct
bind_result
  {
          typedef
        void_ptr_array
        < mpl::package<ArgsPart...>
        >
      part_t;
        part_t
      part_v;
        Functor
      my_ftor;
      bind_result
        ( Functor& a_ftor
        , ArgsPart&... a_args
        )
      : part_v
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
      /**@brief
       *  REQUIRES:
       *    ArgsPart... substituted with
       *    ArgsComplet... produces
       *    a complete argument list.
       *  ENSURES:
       *    returns my_ftor called with
       *    the completed argument list.
       */
      {
          typedef mpl::package<ArgsComplete...> pkg_args;
          typedef void_ptr_array<pkg_args> source_t;
            substitute
            < part_t
            , source_t
            >
          target_complete(part_v,a_args...);
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

      typename Functor::result_type
    operator()
      ( void
      )
      /**@brief
       *  REQUIRES:
       *    ArgsPart... is complete
       *    argument list.
       *  ENSURES:
       *    returns my_ftor called with
       *    the completed argument list.
       */
      {
             composite_storage::pack::multiple_dispatch::
            apply_unpack
            < typename mpl::package_range_c
              < unsigned
              , 0
              , sizeof...(ArgsPart)
              >::type
            >
          uapp;
          return uapp(my_ftor,part_v);
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
  
  template
  < int I
  >
  std::ostream&
operator<<
  ( std::ostream& sout
  , arg<I>
  )
  {
      sout<<"arg<"<<I<<">";
      return sout;
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
  
  void
test()
  {
    int act1=1;
    int act2=2;
    functor_any f_any;
    {
      auto bnd_any=bind(f_any,_1,_2);
      std::cout<<"(_1,_2)("<<act1<<","<<act2<<")\n";
      bnd_any(act1,act2);
    }
    {
      auto bnd_any=bind(f_any,act2,_1);
      std::cout<<"("<<act2<<",_1)("<<act1<<")\n";
      bnd_any(act1);
      std::cout<<"("<<act2<<",_1)("<<act2<<")\n";
      bnd_any(act2);
    }
    {
      auto bnd_any=bind(f_any,act2,act1);
      std::cout<<"("<<act2<<","<<act1<<")()\n";
      bnd_any();
      std::cout<<"("<<act2<<","<<act1<<")("<<act1<<")\n";
      bnd_any(act1);
      std::cout<<"("<<act2<<","<<act1<<")("<<act1<<","<<act2<<")\n";
      bnd_any(act1,act2);
    }
  }  

}//exit sandbox namespace
}//exit boost namespace

int main()
{
  boost::sandbox::test();
  return 0;
}  

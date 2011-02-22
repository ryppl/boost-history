//ptrs_target_source container and function for convering elements from source to target.
#ifndef BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REPLACE_SOURCE_WITH_TARGET_PTR_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REPLACE_SOURCE_WITH_TARGET_PTR_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/mpl/package.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
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
  {
          static
        unsigned const
      size
        =sizeof...(Types)
        ;
          typedef
        void*
      ptr_type
        ;
        ptr_type
      my_ptrs[size]
        ;
    #define VOID_PTR_ARRAY_NO_INIT_LIST defined(__clang__)
    #if VOID_PTR_ARRAY_NO_INIT_LIST
        template
        < typename... Refs
        >
        void
      assign_ptrs
        ( Refs&...refs
        )
      ;
        template
        < typename RefHead
        , typename... RefsTail
        >
        void
      assign_ptrs
        ( RefHead& ref_head
        , RefsTail&... refs_tail
        )
      {
          my_ptrs[sizeof...(Types)-sizeof...(RefsTail)-1]=remove_cv_ptr(ref_head);
          assign_ptrs(refs_tail...);
      }
        void
      assign_ptrs()
      {
      }
    #endif
        //@CTOR
      void_ptr_array
        (   Types&... 
          refs
        )
        #if VOID_PTR_ARRAY_NO_INIT_LIST
        #else
          //g++
        : my_ptrs
          ( { remove_cv_ptr(refs)... }
          )
        #endif
        {
        #if VOID_PTR_ARRAY_NO_INIT_LIST
            assign_ptrs(refs...);
        #endif
        }
    
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
        typename result_type
        < Index
        >::type&
      project(void)const
        {
            ptr_type vp=my_ptrs[Index];
                typedef
              typename result_type<Index>::type
            arg_type;
          #ifdef MULTIPLE_DISPATCH_DEBUG
            result_type<Index>();
            std::cout
              <<"project.\n"
              <<":Index="
              <<Index
              <<"\n"
              <<":arg_type="
              <<utility::demangled_type_name<arg_type>()
              <<"\n"
              ;
          #endif
            arg_type*ap=static_cast<arg_type*>(vp);
            return *ap;
        }
  };

  template
  < typename ArgsTarget //mpl::package of Target types.
  , typename... ArgsSource //Source types.
  >
  struct 
ptrs_target_source
  //Contains pointers for types
  //in ArgsTarget and ArgsSource...
  ;
  
  template
  < typename... ArgsSource
  >
  struct 
ptrs_target_source
  < mpl::package
    < //No Target pointers, yet.
    >
  , ArgsSource...
  >
  /**@brief
   *  Initial value of a ptrs_target_source, i.e.
   *  before any conversion of ArgsSource pointers
   *  to Target pointers.
   */
  : void_ptr_array
    < mpl::package
      < ArgsSource...
      >
    >
  {  
          typedef
        void_ptr_array
        < mpl::package
          < ArgsSource...
          >
        >
      super_type
      ;
      
      ptrs_target_source
        (   ArgsSource&... 
          a_args
        )
        : super_type(a_args...)
        {}
        
      ptrs_target_source
        (   ptrs_target_source const&
          a_self
        )
        : super_type
          ( a_self
          )
        {}
   private:     
      ptrs_target_source(void)
        {}
  };
  template
  <
  >
  struct 
ptrs_target_source
  < mpl::package
    <
    >
  >
  /**@brief
   *  Initial value of a ptrs_target_source, i.e.
   *  before any conversion of ArgsSource pointers
   *  to Target pointers.
   */
  : void_ptr_array
    < mpl::package
      <
      >
    >
  {  
          typedef
        void_ptr_array
        < mpl::package
          <
          >
        >
      super_type
      ;
      
      ptrs_target_source(void)
        {}
  };

  
  template
  < typename... ArgsSource
  >
struct ptrs_target0_source
  /**@brief
   *  Metafunction returning ptrs_target_source
   *  with empty targets
   */
{
        typedef
      ptrs_target_source
      < mpl::package<>
      , ArgsSource...
      >
    type
    ;
};

  template
  < typename... ArgsSource
  >
  typename ptrs_target0_source
  < ArgsSource...
  >::type
mk_ptrs_source
  (   ArgsSource&... 
    a_args
  )
  /**@brief
   *  Convenience function to create an 
   *  "initial" ptrs_target_source, i.e.
   *  one that contains no Target pointers.
   */
  {
       ptrs_target_source
       < mpl::package
         <
         >
       , ArgsSource...
       >
     result_args
       ( a_args...
       )
       ;
     return result_args;
  }   

  typename ptrs_target0_source
  <
  >::type
mk_ptrs_source
  ( void
  )
  /**@brief
   *  Convenience function to create an 
   *  "initial" ptrs_target_source, i.e.
   *  one that contains no Target pointers.
   */
  {
       ptrs_target_source
       < mpl::package
         <
         >
       >
     result_args
       ;
     return result_args;
  }   

  template
  < typename... ArgsTarget
  , typename... ArgsSource
  >
  struct 
ptrs_target_source
  < mpl::package
    < ArgsTarget...
    >
  , ArgsSource...
  >
  : void_ptr_array
    < mpl::package
      < ArgsTarget...
      , ArgsSource...
      >
    >
  {  
   private:
      ptrs_target_source(void);    
      ptrs_target_source(ptrs_target_source const&);    
  };

  template
  < typename... HeadTarget
  , typename TailTarget
  , typename HeadSource
  , typename... TailSource
  >
  ptrs_target_source
  < mpl::package
    < HeadTarget...
    , TailTarget
    >
  , TailSource...
  >*
replace_source_with_target_ptr
  (   ptrs_target_source
      < mpl::package
        < HeadTarget...
        >
      , HeadSource
      , TailSource...
      >*
    old_ptrs
  ,   TailTarget& 
    tail_target
  )
  /**@brief
   *  Replaces the:
   *    HeadSource* 
   *  value within old_ptrs with a:
   *    TailTarget* 
   *  and coerce old_ptr to a pointer to a new
   *  type reflecting that replacement.
   */
  {
      std::size_t const index=sizeof...(HeadTarget);
      old_ptrs->my_ptrs[index]=remove_cv_ptr(tail_target);//replacement
      void*old_pp=old_ptrs;//begin coercion of old_ptrs
          typedef
        ptrs_target_source
        < mpl::package
          < HeadTarget...
          , TailTarget
          >
        , TailSource...
        >
      new_p;//new type reflecting replacement.
      return static_cast<new_p*>(old_pp);//returned completed coercion of old_ptrs
  }
    
}//exit multiple_dispatch namespace
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

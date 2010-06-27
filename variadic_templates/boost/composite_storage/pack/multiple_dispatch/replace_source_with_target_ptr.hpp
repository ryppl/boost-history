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
        //@CTOR
      void_ptr_array
        (   Types&... 
          refs
        )
        : my_ptrs
          { remove_cv_ptr(refs)...
          }
        {
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
        typename result_type<Index>::type&
      project(void)const
        {
            ptr_type vp=my_ptrs[Index];
                typedef
              typename result_type<Index>::type
            arg_type;
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
  ptrs_target_source
  < mpl::package
    < //No target pointers.
    >
  , ArgsSource...
  >
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

  ptrs_target_source
  < mpl::package
    < //No target pointers.
    >
  >
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
   *  Replaces HeadSource pointer with TailTarget pointer
   *  and returns pointer to new type of ptrs_target_source
   *  reflecting that replacement.
   */
  {
      std::size_t const index=sizeof...(HeadTarget);
      typedef typename remove_cv<TailTarget>::type target_rm_cv;
      old_ptrs->my_ptrs[index]=const_cast<target_rm_cv*>(&tail_target);
      void*old_pp=old_ptrs;
          typedef
        ptrs_target_source
        < mpl::package
          < HeadTarget...
          , TailTarget
          >
        , TailSource...
        >
      new_p; 
      return static_cast<new_p*>(old_pp);
  }
    
}//exit multiple_dispatch namespace
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

//help classes removing cv qualifiers from types.
#ifndef BOOST_COMPOSITE_STORAGE_UTILITY_REMOVE_CV_REF_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_UTILITY_REMOVE_CV_REF_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================
namespace boost
{
namespace composite_storage
{
namespace utility
{
  template
  < typename T
  >
struct remove_cv_ref
//Purpose:
//  Remove const and & from T
//MaintenanceNote:2010-03-26
//  using boost::remove_cv<remove_reference<T>::type>::type
//  fails, maybe because that method can't handle rval
//  refs, e.g. T&&.
{
    typedef T type;
};  
  template
  < typename T
  >
struct remove_cv_ref
  < T const
  >
: remove_cv_ref<T>
{
};  
  template
  < typename T
  >
struct remove_cv_ref
  < T const&
  >
: remove_cv_ref<T>
{
};  
  template
  < typename T
  >
struct remove_cv_ref
  < T&&
  >
: remove_cv_ref<T>
{
};  
  template
  < typename T
  >
struct remove_cv_ref
  < T&
  >
: remove_cv_ref<T>
{
};  

}//exit utility namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

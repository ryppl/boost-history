/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_FUNCTOR_HAS_SIGNATURE_HPP_
#define _BOOST_OVERLOAD_DETAIL_FUNCTOR_HAS_SIGNATURE_HPP_


#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include "member_function_trait.hpp"
#include "get_function_tag.hpp"

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// utility to test if a function object has a specific signature

struct member_func_non_const_tag {};

template< typename FunctionObj, typename Sig, 
          typename const_tag = member_func_non_const_tag >
struct func_obj_has_signature
{
  private:
    typedef typename memb_func_trait<Sig FunctionObj::*>::type memb_func_ptr;
    template<memb_func_ptr> struct helper;
    template<typename T> static char check(helper<&T::operator()>* );
    template<typename T> static char (& check(...))[2];
  public:
    static const bool value = ( sizeof(check<FunctionObj>(0)) == sizeof(char) );
};

template<typename FunctionObj>
struct func_obj_has_signature<FunctionObj, no_signature, member_func_non_const_tag>
{
    static const bool value = false;
};

struct member_func_const_tag {}; // for member functions of kind "R f(..) const"

template<typename FunctionObj, typename Sig >
struct func_obj_has_signature<FunctionObj, Sig, member_func_const_tag>
{
  private:
    typedef typename memb_func_trait<Sig FunctionObj::*>::const_type 
            memb_func_ptr;
    template<memb_func_ptr> struct helper;
    template<typename T> static char check(helper<&T::operator()>* );
    template<typename T> static char (& check(...))[2];
  public:
    static const bool value = ( sizeof(check<FunctionObj>(0)) == sizeof(char) );
};

template<typename FunctionObj>
struct func_obj_has_signature<FunctionObj, no_signature, member_func_const_tag>
{
    static const bool value = false;
};

///////////////////////////////////////////////////////////////////////////////
// utility to test if a functor has a specific signature

template< typename Functor, 
          typename Sig,
          typename function_tag 
                = typename get_function_tag<Functor>::type
>
struct functor_has_signature{ static const bool value = false; };

template< typename FunctionPtr, typename Sig>
struct functor_has_signature<FunctionPtr, Sig, function_ptr_tag>
{
    private:
      typedef typename remove_pointer<FunctionPtr>::type func_ptr_t;
    public:
          static const bool value = boost::is_same<func_ptr_t, Sig>::value;
};

template< typename MemberPtr, typename Sig>
struct functor_has_signature<MemberPtr, Sig, member_ptr_tag>
{
  private:
    typedef
        typename memb_func_trait<MemberPtr>::binded_type
        binded_type;
    typedef
        typename memb_func_trait<MemberPtr>::const_binded_type
        const_binded_type;
    static const bool const_qualified 
        = memb_func_trait<MemberPtr>::const_qualified;
  public:
    static const bool value 
        = boost::is_same<binded_type, Sig>::value
        || (boost::is_same<const_binded_type, Sig>::value && const_qualified);
};

template< typename FunctionObj, typename Sig>
struct functor_has_signature<FunctionObj, Sig, function_obj_tag>
{
    static const bool value 
        = func_obj_has_signature<FunctionObj, Sig>::value
        || func_obj_has_signature<FunctionObj, Sig, member_func_const_tag>::value;
};

template< typename FunctionObj, typename Sig>
struct functor_has_signature<boost::reference_wrapper<FunctionObj>, Sig, function_obj_ref_tag>
{
    static const bool value 
        = func_obj_has_signature<FunctionObj, Sig>::value
        || func_obj_has_signature<FunctionObj, Sig, member_func_const_tag>::value;
};

template< typename FuncSig, typename Sig>
struct functor_has_signature< boost::function<FuncSig>, Sig, function_obj_tag >
{
    static const bool value = boost::is_same<FuncSig, Sig>::value;
};

} } } // end namespaces

#endif // _BOOST_OVERLOAD_DETAIL_FUNCTOR_HAS_SIGNATURE_HPP_


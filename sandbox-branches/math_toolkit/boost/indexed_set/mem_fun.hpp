/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_MEM_FUN_HPP
#define BOOST_INDEXED_SET_MEM_FUN_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/mpl/if.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{

template<class T> class reference_wrapper; /* fwd decl. */

namespace indexed_sets{

/* mem_fun implements a read-only key extractor based on a given non-const
 * member function of a class.
 * const_mem_fun does the same for const member functions.
 * Additionally, mem_fun  and const_mem_fun areoverloaded to support
 * referece_wrappers of T and "far pointers" to T's. By far pointer to T we
 * mean a type P such that, given a p of Type P
 *   *...n...*x is convertible to T&, for some n>=1.
 * Examples of far pointers are raw and smart pointers, iterators and
 * arbitrary combinations of these (vg. T** or auto_ptr<T*>.)
 */

template<class Class,typename Type,Type (Class::*PtrToMemberFunction)()const>
struct const_mem_fun
{
  typedef typename remove_reference<Type>::type result_type;

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename FarPtr>
  Type operator()(const FarPtr& x)const
  {
    return operator()(*x);
  }
#endif

  Type operator()(const Class& x)const
  {
    return (x.*PtrToMemberFunction)();
  }

  Type operator()(const reference_wrapper<const Class>& x)const
  { 
    return operator()(x.get());
  }

  Type operator()(const reference_wrapper<Class>& x)const
  { 
    return operator()(x.get());
  }
};

template<class Class,typename Type,Type (Class::*PtrToMemberFunction)()>
struct mem_fun
{
  typedef typename remove_reference<Type>::type result_type;

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename FarPtr>
  Type operator()(const FarPtr& x)const
  {
    return operator()(*x);
  }
#endif

  Type operator()(Class& x)const
  {
    return (x.*PtrToMemberFunction)();
  }

  Type operator()(const reference_wrapper<Class>& x)const
  { 
    return operator()(x.get());
  }
};

/* MSVC++ 6.0 has problems with const member functions as non-type template
 * parameters, somehow it takes them as non-const. mem_fun_explicit workarounds
 * this defficiency by accepting an extra type parameter that specifies the
 * signature of he member function. The workaround was found at:
 *   Daniel, C.:"Re: weird typedef problem in VC", news:microsoft.public.vc.language,
 *   21st nov 2002, 
 *   http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&selm=ukwvg3O0BHA.1512%40tkmsftngp05
 */

template<
  class Class,typename Type,
  typename PtrToMemberFunctionType,PtrToMemberFunctionType PtrToMemberFunction>
struct const_mem_fun_explicit
{
  typedef typename remove_reference<Type>::type result_type;

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename FarPtr>
  Type operator()(const FarPtr& x)const
  {
    return operator()(*x);
  }
#endif

  Type operator()(const Class& x)const
  {
    return (x.*PtrToMemberFunction)();
  }

  Type operator()(const reference_wrapper<const Class>& x)const
  { 
    return operator()(x.get());
  }

  Type operator()(const reference_wrapper<Class>& x)const
  { 
    return operator()(x.get());
  }
};

template<
  class Class,typename Type,
  typename PtrToMemberFunctionType,PtrToMemberFunctionType PtrToMemberFunction>
struct mem_fun_explicit
{
  typedef typename remove_reference<Type>::type result_type;

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename FarPtr>
  Type operator()(const FarPtr& x)const
  {
    return operator()(*x);
  }
#endif

  Type operator()(Class& x)const
  {
    return (x.*PtrToMemberFunction)();
  }

  Type operator()(const reference_wrapper<Class>& x)const
  { 
    return operator()(x.get());
  }
};

/* BOOST_INDEXED_SET_CONST_MEM_FUN and BOOST_INDEXED_SET_MEM_FUN resolve to
 * mem_fun_explicit for MSVC++ 6.0 and to [const_]mem_fun otherwise.
 */

#if defined(BOOST_MSVC)&&(BOOST_MSVC<1300)

#define BOOST_INDEXED_SET_CONST_MEM_FUN(Class,Type,MemberFunName) \
::boost::indexed_sets::const_mem_fun_explicit<\
  Class,Type,Type (Class::*)()const,&Class::MemberFunName>
#define BOOST_INDEXED_SET_MEM_FUN(Class,Type,MemberFunName) \
::boost::indexed_sets::mem_fun_explicit<\
  Class,Type,Type (Class::*)(),&Class::MemberFunName>

#else

#define BOOST_INDEXED_SET_CONST_MEM_FUN(Class,Type,MemberFunName) \
::boost::indexed_sets::const_mem_fun<Class,Type,&Class::MemberFunName>
#define BOOST_INDEXED_SET_MEM_FUN(Class,Type,MemberFunName) \
::boost::indexed_sets::mem_fun<Class,Type,&Class::MemberFunName>

#endif

} /* namespace indexed_sets */

} /* namespace boost */

#endif

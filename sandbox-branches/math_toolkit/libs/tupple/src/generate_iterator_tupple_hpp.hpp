
// Copyright (C) 2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#if 0
// This file is indented to generate "iterator_tupple.hpp".
// Do not include this file directly, but rather #include "boost/tupple/iterator_tupple.hpp"

// Comments which should not appear in the output are surrounded
// with #if 0 - #endif blocks throughout this file.
#endif

#define PREPROCESS_LATER(arg) arg

#ifdef TUPPLE_PARTIAL_SPEC
PREPROCESS_LATER(#ifndef BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_PART_SPEC_HPP)
PREPROCESS_LATER(#define BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_PART_SPEC_HPP)
#else
PREPROCESS_LATER(#ifndef BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_NO_PART_SPEC_HPP)
PREPROCESS_LATER(#define BOOST_TUPPLE_DETAIL_ITERATOR_TUPPLE_NO_PART_SPEC_HPP)
#endif

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

PREPROCESS_LATER(#include <boost/tupple/tupple.hpp>)
PREPROCESS_LATER(#include <boost/tupple/detail/iterator_tupple_traits_detail.hpp>)
PREPROCESS_LATER(#include <boost/tupple/detail/ref_workaround.hpp>)

PREPROCESS_LATER(#include <boost/utility.hpp>)

#ifndef TUPPLE_PARTIAL_SPEC
  PREPROCESS_LATER(#include <boost/type_traits.hpp>)
  PREPROCESS_LATER(#include <boost/mpl/if.hpp>)
  PREPROCESS_LATER(#include <boost/static_assert.hpp>)
#endif

namespace boost {
namespace tupple {

#define IF_INT0(T,E) E
#define IF_INT1(T,E) T
#define IF_INT2(T,E) T
#define IF_INT3(T,E) T
#define IF_INT4(T,E) T
#define IF_INT5(T,E) T
#define IF_INT6(T,E) T
#define IF_INT7(T,E) T
#define IF_INT8(T,E) T
#define IF_INT9(T,E) T


#if 0
//
// === Global definitions used throughout the file
//
#endif

#define MAX_N 10

#define MYELEM0 a
#define MYELEM1 b
#define MYELEM2 c
#define MYELEM3 d
#define MYELEM4 e
#define MYELEM5 f
#define MYELEM6 g
#define MYELEM7 h
#define MYELEM8 i
#define MYELEM9 j

#define THEMYELEM0 theA
#define THEMYELEM1 theB
#define THEMYELEM2 theC
#define THEMYELEM3 theD
#define THEMYELEM4 theE
#define THEMYELEM5 theF
#define THEMYELEM6 theG
#define THEMYELEM7 theH
#define THEMYELEM8 theI
#define THEMYELEM9 theJ

#define _DELAY_(fct,arg)  __DELAY__(fct,arg)
#define __DELAY__(fct,arg)  fct##arg

#ifdef TUPPLE_PARTIAL_SPEC
#  define TUPLE(k) tuple
#  define ITERTUPLE(k) iterator_tuple
#else
#  define TUPLE(k) tuple##k
#  define ITERTUPLE(k) iterator_tuple##k
#endif


#define NULLTYPE null_type

#if 0
#  define ELEM(k,A) MYELEM##k
#  define THEELEM(k,A) THEMYELEM##k
#else
#  define ELEM(k,A) m##k
#  define THEELEM(k,A) theM##k
#endif

#define THEELEMS(z,k,A) THEELEM(k,A)

#define TYPE(name,k,arg) name<BOOST_PP_ENUM_PARAMS(k,arg)>
#define TEMPLATE(k,arg) template<BOOST_PP_ENUM_PARAMS(k,class arg)>
#define TEMPLATENULL(k,arg) template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(k,class arg,NULLTYPE)>

#define NULLTYPES(z,k,_) NULLTYPE

#define CTORARG(z,k,_) ARGTYPE(k) THEELEM(k,_)

#define NTIMES(z,k,arg) arg

#define ARGTYPE(k) arg_type##k

#define OP_IF(c,op) BOOST_PP_IF(c,op,BOOST_PP_EMPTY())


#ifdef TUPPLE_PARTIAL_SPEC
#  define TEMPLATESPEC(k,r,arg) < BOOST_PP_ENUM_PARAMS(k,arg) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,_) >
#else
#  define TEMPLATESPEC(k,r,arg)
#endif


#ifdef TUPPLE_PARTIAL_SPEC
#  define TEMPLATESPEC1(a,k,r)    < a, BOOST_PP_ENUM_PARAMS(k,T) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,_) >
#  define TEMPLATESPEC2(a,b,k,r)  < a, b, BOOST_PP_ENUM_PARAMS(k,T) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,_) >
#else
#  define TEMPLATESPEC1(a,k,r)
#  define TEMPLATESPEC2(a,b,k,r)
#endif

#if 0
//
// === Definitions which container_tuple and zip_view share
//
#endif

#define TYPE2(name,k,a,b) name<BOOST_PP_ENUM_PARAMS(k,a),BOOST_PP_ENUM_PARAMS(k,b)>

#define INC(z,k,_) ++ELEM(k,A);
#define DEC(z,k,_) --ELEM(k,A);


#define ITERTRAITS_CATEGORY(z,k,arg)  typename arg##k::iterator_category
#define ITERTRAITS_VALUE(z,k,arg)     typename arg##k::value_type

#ifdef TUPPLE_PARTIAL_SPEC
#  define ITERTRAITS_REFERENCE(z,k,arg)  typename arg##k::reference
#  define ITERTRAITS_CONSTREFERENCE(z,k,arg) const typename arg##k::reference
#else
#  define ITERTRAITS_REFERENCE(z,k,arg)  detail::reference_wrapper< ITERTRAITS_VALUE(z,k,arg) >
#  define ITERTRAITS_CONSTREFERENCE(z,k,arg) detail::reference_wrapper< ITERTRAITS_VALUE(z,k,arg) const>
#endif

#define ITERTRAITS_POINTER(z,k,arg)    typename arg##k::pointer
#define ITERTRAITS_DIFFERENCE(z,k,arg)  typename arg##k::difference_type


#ifdef TUPPLE_PARTIAL_SPEC
#  define DEREF(z,k,_) *ELEM(k,A)
#  define CONSTDEREF(z,k,_) *ELEM(k,_)
#  define ARRAYACCESS(z,k,arg) ELEM(k,A)[arg.ELEM(k,A)]
#  define CONSTARRAYACCESS(z,k,arg) ELEM(k,A)[arg.ELEM(k,A)]
#else
#  define DEREF(z,k,_) detail::ref( *ELEM(k,A) )
#  define CONSTDEREF(z,k,_) detail::cref( *ELEM(k,A) )
#  define ARRAYACCESS(z,k,arg) detail::ref(ELEM(k,A)[arg.ELEM(k,A)])
#  define CONSTARRAYACCESS(z,k,arg) detail::cref(ELEM(k,A)[arg.ELEM(k,A)])
#endif


#define POINTTO(z,k,_) ::boost::addressof( *ELEM(k,_) )


#if 0
//
// === Part 4: Derived tuple classes for iterators and containers
//
#endif


#define PLUS2(z,k,_) ELEM(k,_) += rhs.ELEM(k,_);
#define MINUS2(z,k,_) ELEM(k,_) -= rhs.ELEM(k,_);

#define DIFF(z,k,_) lhs.ELEM(k,_) - rhs.ELEM(k,_)

#ifdef TUPPLE_PARTIAL_SPEC
#  define ITERTRAITS(k) iterator_traits_tuple
#  define ITERSELFTYPE(k) TYPE(ITERTUPLE(k),k,T)
#  define DETAILTRAITS(z,k,_) detail::iterator_traits<T##k>
#  define ITERTRAITSTYPE(k) ITERTRAITS(k)<BOOST_PP_ENUM(k,DETAILTRAITS,_)>
#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a)>
#  define TRAITSDEFAULT(k,A)
#else
#  define ITERTRAITS(k) iterator_traits_tuple##k
#  define ITERSELFTYPE(k) TYPE(ITERTUPLE(k),k,T)
#  define DETAILTRAITS(z,k,_) detail::iterator_traits<T##k>
#  define ITERTRAITSTYPE(k) ITERTRAITS(k)<BOOST_PP_ENUM(k,DETAILTRAITS,_)>
#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a)>
#  define TRAITSDEFAULT(k,A)
#endif

#if 0
// Still missing: operator->(), which I couldn't figure out to implement properly yet.
#endif


#define ITERATOR_TUPLE(k,r)                                                       \
TEMPLATE(k,R) struct ITERTRAITS(k) TEMPLATESPEC(k,r,R) {                          \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_CATEGORY,R)>   iterator_category;   \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_VALUE,R)>      value_type;          \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_REFERENCE,R)>  reference;           \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_CONSTREFERENCE,R)> const_reference; \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_POINTER,R)>    pointer;             \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_DIFFERENCE,R)> difference_type;     \
};                                                                                \
                                                                                  \
TRAITSTEMPLATE( k,T,TRAITSDEFAULT ) struct ITERTUPLE(k) TEMPLATESPEC(k,r,T)       \
  : public TYPE(TUPLE(k),k,T)                                             \
{                                                                         \
  typedef ITERSELFTYPE(k)    self_type;                                   \
  typedef TYPE(TUPLE(k),k,T) parent_type;                                 \
  typedef ITERTRAITSTYPE(k) traits;                                       \
                                                                          \
  typedef typename traits::iterator_category iterator_category;           \
  typedef typename traits::value_type        value_type;                  \
  typedef typename traits::reference         reference;                   \
  typedef typename traits::const_reference   const_reference;             \
  typedef typename traits::pointer           pointer;                     \
  typedef typename traits::difference_type   difference_type;             \
                                                                          \
  ITERTUPLE(k)() {}                                                       \
                                                                          \
  ITERTUPLE(k)(BOOST_PP_ENUM(k,CTORARG,_))                                \
  : parent_type( BOOST_PP_ENUM(k,THEELEMS,A) ) {}                         \
                                                                          \
  TEMPLATE(k,S) ITERTUPLE(k)( const TYPE(ITERTUPLE(k),k,S)& rhs )         \
  : parent_type( rhs ) {}                                                 \
                                                                          \
  ITERTUPLE(k)( CTORARG(z,0,_), const tail_type& tail )                   \
  : parent_type( THEELEM(0,A), tail ) {}                                  \
                                                                          \
  self_type& operator++() {                                               \
    BOOST_PP_REPEAT(k,INC,_) return *this;                                \
  }                                                                       \
                                                                          \
  const self_type operator++(int) {                                       \
    self_type old = *this; ++(*this); return old;                         \
  }                                                                       \
                                                                          \
  self_type& operator+=( const difference_type& rhs ) {                   \
    BOOST_PP_REPEAT(k,PLUS2,_) return *this;                              \
  }                                                                       \
                                                                          \
  self_type& operator--() {                                               \
    BOOST_PP_REPEAT(k,DEC,_) return *this;                                \
  }                                                                       \
                                                                          \
  const self_type operator--(int) {                                       \
    self_type old = *this; --(*this); return old;                         \
  }                                                                       \
                                                                          \
  self_type& operator-=( const difference_type& rhs ) {                   \
    BOOST_PP_REPEAT(k,MINUS2,_) return *this;                             \
  }                                                                       \
                                                                          \
  reference operator*() {                                                 \
    return reference( BOOST_PP_ENUM(k,DEREF,_) );                         \
  }                                                                       \
                                                                          \
  const_reference operator*() const {                                     \
    return const_reference( BOOST_PP_ENUM(k,CONSTDEREF,_) );              \
  }                                                                       \
                                                                          \
  reference operator[]( const difference_type& i ) {                      \
    return reference( BOOST_PP_ENUM(k,ARRAYACCESS,i) );                   \
  }                                                                       \
                                                                          \
  const_reference operator[]( const difference_type& i ) const {          \
    return const_reference( BOOST_PP_ENUM(k,CONSTARRAYACCESS, i) );       \
  }                                                                       \
};                                                                        \
                                                                          \
template<BOOST_PP_ENUM_PARAMS(k,class T)>                                 \
TYPE(ITERTUPLE(k),k,T) operator+(                                         \
  const TYPE(ITERTUPLE(k),k,T)& lhs,                                      \
  const TYPE(ITERTUPLE(k),k,T)::difference_type& rhs )                    \
{ return TYPE(ITERTUPLE(k),k,T)(lhs) += rhs; }                            \
                                                                          \
template<BOOST_PP_ENUM_PARAMS(k,class T)>                                 \
TYPE(ITERTUPLE(k),k,T) operator-(                                         \
  const TYPE(ITERTUPLE(k),k,T)& lhs,                                      \
  const TYPE(ITERTUPLE(k),k,T)::difference_type& rhs )                    \
{ return TYPE(ITERTUPLE(k),k,T)(lhs) -= rhs; }                            \
                                                                          \
template<BOOST_PP_ENUM_PARAMS(k,class T)>                                 \
TYPE(ITERTUPLE(k),k,T)::difference_type operator-(                        \
  const TYPE(ITERTUPLE(k),k,T)& lhs,                                      \
  const TYPE(ITERTUPLE(k),k,T)& rhs )                                     \
{ return TYPE(ITERTUPLE(k),k,T)::difference_type( BOOST_PP_ENUM(k,DIFF,_) ); }


#define TRAITSDEFAULT2(z,k,_) class R##k = detail::iterator_traits<typename T##k::iterator>


#ifdef TUPPLE_PARTIAL_SPEC
TEMPLATENULL(10,T) struct ITERTUPLE(10) { };
TEMPLATENULL(10,R) struct ITERTRAITS(10) { };
#endif

#ifdef TUPPLE_PARTIAL_SPEC
template<> struct ITERTUPLE(0) TEMPLATESPEC(0,10,T) : public tuple<null_type>
{
};
#else
template<class T> struct ITERTUPLE(0)
{
  typedef ITERTUPLE(0)<T> self_type;
};
#endif


ITERATOR_TUPLE(1,9)
ITERATOR_TUPLE(2,8)
ITERATOR_TUPLE(3,7)
ITERATOR_TUPLE(4,6)
ITERATOR_TUPLE(5,5)
ITERATOR_TUPLE(6,4)
ITERATOR_TUPLE(7,3)
ITERATOR_TUPLE(8,2)
ITERATOR_TUPLE(9,1)

#ifdef TUPPLE_PARTIAL_SPEC

template<class T, int N> struct n_fold_iterator_tuple { };

#define N_FOLD_ITERTUPLE(z,k,t)                                     \
template<class t> struct n_fold_iterator_tuple<t,k> {               \
  typedef ITERTUPLE(k)<BOOST_PP_ENUM(k,NTIMES,t)> type;             \
                                                                    \
  static type make( const t& arg )                                  \
  { return type(BOOST_PP_ENUM(k,NTIMES,arg)); }                     \
};

BOOST_PP_REPEAT( 10, N_FOLD_ITERTUPLE, T )

#else

namespace detail
{
  #define N_FOLD_ITER_SELECT(z,k,t)                                   \
  template<> struct iter_select<k>                                    \
  {                                                              \
     typedef ITERTUPLE(k)<BOOST_PP_ENUM(k,NTIMES,t)> type;       \
                                                                 \
     static type make( const t& arg )                            \
     { return type(BOOST_PP_ENUM(k,NTIMES,arg)); }               \
  };


  template<class T> struct n_fold_iterator_helper
  {
    template<int N> struct iter_select {};
    template<> struct iter_select<0> { typedef ITERTUPLE(0)<null_type> type; };
    BOOST_PP_REPEAT_FROM_TO( 1, 10, N_FOLD_ITER_SELECT, T )
  };
} // namespace detail

template<class T, int N> struct n_fold_iterator_tuple
{
  typedef detail::n_fold_iterator_helper<T>::iter_select<N>::type type;

  static type make( const T& arg )
  { return detail::n_fold_iterator_helper<T>::iter_select<N>::make( arg ); }
};

#endif


#ifndef TUPPLE_PARTIAL_SPEC

  namespace detail {

    #define BASE_TYPE_SELECTOR(k) _DELAY_(iterator_tuple_base_type_selector,k)
    #define TEE(k) _DELAY_(T,k)

    #define BASE_TYPE_SELECT(k)                                               \
    TEMPLATE(k,T) struct BASE_TYPE_SELECTOR(k)                                \
    {                                                                         \
      typedef ::boost::mpl::if_c<                                             \
        ::boost::is_same< TEE(BOOST_PP_DEC(k)), null_type >::value,           \
          BASE_TYPE_SELECTOR(BOOST_PP_DEC(k))                                 \
           < BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(k), T ) > ::type,             \
          ITERTUPLE(k)< BOOST_PP_ENUM_PARAMS( k, T ) >                        \
      >::type type;                                                           \
    };

    // The initial case.
    template< class T0 >
    struct BASE_TYPE_SELECTOR(1)
    {
      typedef ::boost::mpl::if_c<
        ::boost::is_same< T0, null_type >::value,
          ITERTUPLE(0)< null_type >,
          ITERTUPLE(1)< T0 >
      >::type type;
    };

    BASE_TYPE_SELECT(2)
    BASE_TYPE_SELECT(3)
    BASE_TYPE_SELECT(4)
    BASE_TYPE_SELECT(5)
    BASE_TYPE_SELECT(6)
    BASE_TYPE_SELECT(7)
    BASE_TYPE_SELECT(8)
    BASE_TYPE_SELECT(9)

  } // namespace detail


  #define VEEVEE(z,k,_) V##k v##k
  #define VEE(z,k,_) v##k

  #define REFLECT_CTOR(k)                                               \
    TEMPLATE(k,V) iterator_tuple( BOOST_PP_ENUM( k, VEEVEE, _ ) ):      \
      self_type( BOOST_PP_ENUM( k, VEE, _ )  )                          \
    {}

  TEMPLATENULL(9,T) struct iterator_tuple:
    public detail::iterator_tuple_base_type_selector9< BOOST_PP_ENUM_PARAMS( 9, T ) >::type
  {
    typedef detail::iterator_tuple_base_type_selector9< BOOST_PP_ENUM_PARAMS( 9, T ) >::type base_type;

    iterator_tuple()
      : self_type()
    {}

    typedef typename base_type::iterator_category iterator_category;
    typedef typename base_type::value_type        value_type;
    typedef typename base_type::reference         reference;
    typedef typename base_type::const_reference   const_reference;
    typedef typename base_type::pointer           pointer;
    typedef typename base_type::difference_type   difference_type;

    REFLECT_CTOR(1)
    REFLECT_CTOR(2)
    REFLECT_CTOR(3)
    REFLECT_CTOR(4)
    REFLECT_CTOR(5)
    REFLECT_CTOR(6)
    REFLECT_CTOR(7)
    REFLECT_CTOR(8)
    REFLECT_CTOR(9)

    TEMPLATE(9,S) base_type&
      operator=( const iterator_tuple< BOOST_PP_ENUM_PARAMS( 9, S ) >& rhs )
    {
      //BOOST_STATIC_ASSERT(  );
      return( base_type::operator=( rhs ) );
    }
  };

#endif

#if 0
// Should the return type be tuple<T0,T1,...>,  tuple<S0,S1,...>, or something different?
#endif

#define BINOP(op,k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),class S>   \
TYPE(ITERTUPLE(k),k,T) operator##op( const TYPE(ITERTUPLE(k),k,T)& lhs, const S& rhs ) \
{ return TYPE(ITERTUPLE(k),k,T)(lhs) op##= rhs; }

} // namespace tupple
} // namespace boost


PREPROCESS_LATER(#endif)

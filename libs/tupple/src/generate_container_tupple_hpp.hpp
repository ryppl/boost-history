
// Copyright (C) 2001,2002 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#if 0
// This file is indented to generate "container_tupple.hpp".
// Do not include this file directly, but rather #include "boost/tupple/container_tupple.hpp"

// Comments which should not appear in the output are surrounded
// with #if 0 - #endif blocks throughout this file.
#endif

#define PREPROCESS_LATER(arg) arg

#ifdef TUPPLE_PARTIAL_SPEC
PREPROCESS_LATER(#ifndef BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_PART_SPEC_HPP)
PREPROCESS_LATER(#define BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_PART_SPEC_HPP)
#else
PREPROCESS_LATER(#ifndef BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_NO_PART_SPEC_HPP)
PREPROCESS_LATER(#define BOOST_TUPPLE_DETAIL_CONTAINER_TUPPLE_NO_PART_SPEC_HPP)
#endif

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>


PREPROCESS_LATER(#include "boost/tupple/tupple.hpp")
PREPROCESS_LATER(#include "boost/tupple/detail/container_tupple_traits_detail.hpp")


namespace boost {
namespace tupple {

#if 0
//===== Begin of tupple code

// We need `IF' for ints, is there a more elegant possibility?
#endif

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

#ifdef TUPPLE_PARTIAL_SPEC
#  define TUPLE(k) tuple
#  define ITERTUPLE(k) iterator_tuple
#  define CONTTUPLE(k) container_tuple
#  define N_FOLD_ITER(a,k) n_fold_iterator<a,k>
#  define N_FOLD_CONT(a,k) n_fold_container<a,k>
#else
#  define TUPLE(k) tuple##k
#  define ITERTUPLE(k) iterator_tuple##k
#  define CONTTUPLE(k) container_tuple##k
#  define N_FOLD_ITER(a,k) n_fold_iterator##k
#  define N_FOLD_CONT(a,k) n_fold_container##k
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

#define PLUS1(k,arg) ELEM(k,A) += arg;
#define MINUS1(k,A) ELEM(k,A) -= rhs;


#define ITERTRAITS_CATEGORY(z,k,arg)  typename arg##k::iterator_category
#define ITERTRAITS_VALUE(z,k,arg)     typename arg##k::value_type

#if 0
// Dereferencing would require passing tuple<A&,B&>
// --> use assign_to_pointee
#endif

#ifdef TUPPLE_PARTIAL_SPEC
#  define ITERTRAITS_REFERENCE(z,k,arg)  typename arg##k::reference
#  define ITERTRAITS_CONSTREFERENCE(z,k,arg) const typename arg##k::reference
#else
#  define ITERTRAITS_REFERENCE(z,k,arg)  detail::assign_to_pointee<typename arg##k::value_type>
#  define ITERTRAITS_CONSTREFERENCE(z,k,arg) typename arg##k::value_type
#endif

#define ITERTRAITS_POINTER(z,k,arg)    typename arg##k::pointer
#define ITERTRAITS_DIFFERENCE(z,k,arg)  typename arg##k::difference_type


#ifdef TUPPLE_PARTIAL_SPEC
#  define DEREF(z,k,_) *ELEM(k,A)
#  define ARRAYACCESS(z,k,arg) ELEM(k,A)[arg.ELEM(k,A)]
#  define CONSTARRAYACCESS(z,k,arg) ELEM(k,A)[arg.ELEM(k,A)]
#else
#  define DEREF(z,k,_) ELEM(k,A)
#  define ARRAYACCESS(z,k,arg) &(ELEM(k,A)[arg.ELEM(k,A)])
#  define CONSTARRAYACCESS(z,k,arg) ELEM(k,A)[arg.ELEM(k,A)]
#endif

#define CONSTDEREF(z,k,_) *ELEM(k,A)
#define POINTTO(z,k,A) ELEM(k,A)

#define TYPENAME_VALUE(z,k,arg)          typename arg##k::value_type
#define TYPENAME_ITERATOR(z,k,arg)       typename arg##k::iterator
#define TYPENAME_CONSTITERATOR(z,k,arg)  typename arg##k::const_iterator

#ifdef TUPPLE_PARTIAL_SPEC
#  define TYPENAME_REFERENCE(z,k,arg)      typename arg##k::reference
#  define TYPENAME_CONSTREFERENCE(z,k,arg) typename arg##k::const_reference
#else
#  define TYPENAME_REFERENCE(z,k,arg)      detail::assign_to_pointee<typename arg##k::value_type>
#  define TYPENAME_CONSTREFERENCE(z,k,arg) typename arg##k::value_type
#endif

#define TYPENAME_POINTER(z,k,arg)        typename arg##k::pointer
#define TYPENAME_DIFFERENCE(z,k,arg)     typename arg##k::difference_type
#define TYPENAME_SIZETYPE(z,k,arg)       typename arg##k::size_type

#if 0
//
// === Part 4: Derived tuple classes for iterators and containers
//
#endif

#define MINMAX(k) \
template<class T> T minimal( const TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)>& t ) {  \
  return std::min( t.head(), minimal( t.tail() ) );    \
}                                                       \
template<class T> T maximal( const TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)>& t ) {  \
  return std::max( t.head(), maximal( t.tail() ) );    \
}

#if 0
template<class T> T minimal( const TUPLE(1)<T>& t ) { return t.head(); }
template<class T> T maximal( const TUPLE(1)<T>& t ) { return t.head(); }

MINMAX(2)
MINMAX(3)
MINMAX(4)
MINMAX(5)
MINMAX(6)
MINMAX(7)
MINMAX(8)
MINMAX(9)
#endif

#undef MINMAX


#define PLUS2(z,k,_) ELEM(k,A) += rhs.ELEM(k,A);
#define MINUS2(z,k,_) ELEM(k,A) -= rhs.ELEM(k,A);

#ifdef TUPPLE_PARTIAL_SPEC
#  define ITERTRAITS(k) iterator_traits_tuple
#  define DETAILTRAITS(z,k,_) detail::iterator_traits<T##k>
#  define ITERTRAITSTYPE(k) ITERTRAITS(k)<BOOST_PP_ENUM(k,DETAILTRAITS,_)>
#  define ITERSELFTYPE(k) TYPE(ITERTUPLE(k),k,T)

#  define CONTTRAITS(k) container_traits_tuple
#  define DETAILTRAITS2(z,k,_) detail::iterator_traits<typename T##k::iterator>
#  define CONTTRAITSTYPE(k) CONTTRAITS(k)<BOOST_PP_ENUM(k,DETAILTRAITS2,_)>

#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a)>
#  define TRAITSDEFAULT(k,A)
#else
#  define ITERTRAITS(k) iterator_traits_tuple##k
#  define ITERSELFTYPE(k) TYPE2(ITERTUPLE(k),k,T,R)
#  define ITERTRAITSTYPE(k) TYPE( ITERTRAITS(k),k,R )

#  define CONTTRAITS(k) container_traits_tuple##k
#  define CONTTRAITSTYPE(k) TYPE( CONTTRAITS(k),k,R )

#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a),BOOST_PP_ENUM(k,b,_)>
#  define TRAITSDEFAULT(z,k,_) class R##k = detail::iterator_traits<T##k>
#endif



#if 0
// Add sometimes:

  pointer operator->() {                                                  \
    return pointer( BOOST_PP_ENUM(k,POINTTO,A) );                       \
  }

#endif

#define ITERATOR_TUPLE(k,r)                                               \
TEMPLATE(k,R) struct ITERTRAITS(k) TEMPLATESPEC(k,r,R) {                 \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_CATEGORY,R)>   iterator_category;   \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_VALUE,R)>      value_type;          \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_REFERENCE,R)>  reference;           \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_CONSTREFERENCE,R)> const_reference; \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_POINTER,R)>    pointer;             \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,ITERTRAITS_DIFFERENCE,R)> difference_type;     \
};                                                                                  \
                                                                                    \
TRAITSTEMPLATE( k,T,TRAITSDEFAULT ) struct ITERTUPLE(k) TEMPLATESPEC(k,r,T)        \
  : public TYPE(TUPLE(k),k,T)                                             \
{                                                                         \
  typedef ITERSELFTYPE(k) self_type;                                      \
  typedef ITERTRAITSTYPE(k) traits;                                       \
                                                                          \
  typedef typename traits::iterator_category iterator_category;           \
  typedef typename traits::value_type        value_type;                  \
  typedef typename traits::reference         reference;                   \
  typedef typename traits::const_reference   const_reference;             \
  typedef typename traits::pointer           pointer;                     \
  typedef typename traits::difference_type   difference_type;             \
                                                                          \
  ITERTUPLE(k)(BOOST_PP_ENUM(k,CTORARG,_))                                \
  : TYPE(TUPLE(k),k,T)( BOOST_PP_ENUM(k,THEELEMS,A) ) {}                  \
                                                                          \
  TEMPLATE(k,S) ITERTUPLE(k)( const TYPE(ITERTUPLE(k),k,S)& rhs )         \
  : TYPE(TUPLE(k),k,T)( rhs ) {}                                          \
                                                                          \
  ITERTUPLE(k)( CTORARG(z,0,_), const tail_type& tail )                   \
  : TYPE(TUPLE(k),k,T)( THEELEM(0,A), tail ) {}                           \
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
template<class T> struct N_FOLD_ITER(T,k) {                               \
  typedef ITERTUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)> type;                 \
};


#define EMPTY(z,k,_) ELEM(k,_).empty()
#define SIZE(z,k,_)  ELEM(k,_).size()
#define BEGIN(z,k,_) ELEM(k,_).begin()
#define END(z,k,_)   ELEM(k,_).end()

#define TRAITSDEFAULT2(z,k,_) class R##k = detail::iterator_traits<typename T##k::iterator>


#ifdef TUPPLE_PARTIAL_SPEC
#define ITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_ITERATOR,R)>
#define CONSTITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_CONSTITERATOR,R)>
#else
#define ITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_ITERATOR,R),BOOST_PP_ENUM_PARAMS(k,R)>
#define CONSTITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_CONSTITERATOR,R),BOOST_PP_ENUM_PARAMS(k,R)>
#endif


#define CONTAINER_TUPLE(k,r)                                              \
TEMPLATE(k,R) struct CONTTRAITS(k) TEMPLATESPEC(k,r,R) {                  \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_VALUE,R)>             value_type;       \
  typedef ITERTUPLEDEF(k)                                         iterator;         \
  typedef CONSTITERTUPLEDEF(k)                                    const_iterator;   \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_REFERENCE,R)>         reference;        \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_CONSTREFERENCE,R)>    const_reference;  \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_POINTER,R)>           pointer;          \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_DIFFERENCE,R)>        difference_type;  \
};                                                                        \
                                                                          \
TRAITSTEMPLATE( k,T,TRAITSDEFAULT2 )  struct CONTTUPLE(k) TEMPLATESPEC(k,r,T)   \
  : public TYPE(TUPLE(k),k,T)                                             \
{                                                                         \
  typedef TYPE(CONTTUPLE(k),k,T) self_type;                               \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_SIZETYPE,T)> size_type;       \
  typedef CONTTRAITSTYPE(k) traits;                                       \
                                                                          \
  typedef typename traits::value_type        value_type;                  \
  typedef typename traits::iterator          iterator;                    \
  typedef typename traits::const_iterator    const_iterator;              \
  typedef typename traits::reference         reference;                   \
  typedef typename traits::const_reference   const_reference;             \
  typedef typename traits::pointer           pointer;                     \
  typedef typename traits::difference_type   difference_type;             \
                                                                          \
  CONTTUPLE(k)(BOOST_PP_ENUM(k,CTORARG,_))                                \
  : TYPE(TUPLE(k),k,T)( BOOST_PP_ENUM(k,THEELEMS,A) ) {}                  \
                                                                          \
  TEMPLATE(k,S) CONTTUPLE(k)( const TYPE(ITERTUPLE(k),k,S)& rhs )         \
  : TYPE(TUPLE(k),k,T)( rhs ) {}                                          \
                                                                          \
  CONTTUPLE(k)( CTORARG(z,0,_), const tail_type& tail )                   \
  : TYPE(TUPLE(k),k,T)( THEELEM(0,A), tail ) {}                           \
                                                                          \
  TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,bool)> empty() const {                  \
    return TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,bool)>(                        \
                              BOOST_PP_ENUM(k,EMPTY,_) );                 \
  }                                                                       \
                                                                          \
  size_type size() const {                                                \
    return size_type( BOOST_PP_ENUM(k,SIZE,A) );                          \
  }                                                                       \
                                                                          \
  iterator begin() {                                                      \
    return iterator( BOOST_PP_ENUM(k,BEGIN,A) );                          \
  }                                                                       \
                                                                          \
  const_iterator begin() const {                                          \
    return const_iterator( BOOST_PP_ENUM(k,BEGIN,A) );                    \
  }                                                                       \
                                                                          \
  iterator end() {                                                        \
    return iterator( BOOST_PP_ENUM(k,END,A) );                            \
  }                                                                       \
                                                                          \
  const_iterator end() const {                                            \
    return const_iterator( BOOST_PP_ENUM(k,END,A) );                      \
  }                                                                       \
                                                                          \
  reference operator[]( const size_type& i ) {                            \
    return reference( BOOST_PP_ENUM(k,ARRAYACCESS,i) );                   \
  }                                                                       \
                                                                          \
  const_reference operator[]( const size_type& i ) const {                \
    return const_reference( BOOST_PP_ENUM(k,CONSTARRAYACCESS,i) );        \
  }                                                                       \
};                                                                        \
                                                                          \
template<class T> struct N_FOLD_CONT(T,k) {                               \
  typedef CONTTUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)> type;                   \
};

#if 0
// The add function belongs to a 'mathematical' vector_tuple type.

#define ADD(z,k,arg) lhs + arg.ELEM(k,A)

template<class IterT,class DiffT>                                         \
tuple<BOOST_PP_ENUM(k,NTIMES,IterT)> add(                             \
  const IterT& lhs,                                                       \
  const tuple<BOOST_PP_ENUM(k,NTIMES,DiffT)>& rhs )                   \
{ return tuple<BOOST_PP_ENUM(k,NTIMES,IterT)>( BOOST_PP_ENUM(k,ADD,rhs) ); }

#endif

#ifdef TUPPLE_PARTIAL_SPEC
TEMPLATENULL(10,T) struct ITERTUPLE(10) { };
template<class T, int N> struct n_fold_iterator { };
TEMPLATENULL(10,R) struct ITERTRAITS(10) { };
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
TEMPLATENULL(10,T) struct CONTTUPLE(k) { };
template<class T, int N> struct n_fold_container { };
TEMPLATENULL(10,R) struct CONTTRAITS(10) { };
#endif

CONTAINER_TUPLE(1,9)
CONTAINER_TUPLE(2,8)
CONTAINER_TUPLE(3,7)
CONTAINER_TUPLE(4,6)
CONTAINER_TUPLE(5,5)
CONTAINER_TUPLE(6,4)
CONTAINER_TUPLE(7,3)
CONTAINER_TUPLE(8,2)
CONTAINER_TUPLE(9,1)


#if 0
// Should the return type be tuple<T0,T1,...>,  tuple<S0,S1,...>, or something different?
#endif

#define BINOP(op,k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),class S>   \
TYPE(ITERTUPLE(k),k,T) operator##op( const TYPE(ITERTUPLE(k),k,T)& lhs, const S& rhs ) \
{ return TYPE(ITERTUPLE(k),k,T)(lhs) op##= rhs; }

#undef BINOP

#undef PLUS2
#undef MINUS2

#undef ADD

#undef ITERATOR_TUPLE
#undef CONTAINER_TUPLE



#undef INC
#undef DEC

#undef PLUS1
#undef MINUS1

#undef DEREF
#undef CONSTDEREF
#undef ARRAYACCESS
#undef CONSTARRAYACCESS

#undef EMPTY
#undef SIZE
#undef BEGIN
#undef END


#undef ITERTRAITS_CATEGORY
#undef ITERTRAITS_VALUE
#undef ITERTRAITS_REFERENCE
#undef ITERTRAITS_POINTER
#undef ITERTRAITS_DIFFERENCE
#undef ITERTRAITS_CONSTREFERENCE

#undef TYPENAME_VALUE
#undef TYPENAME_ITERATOR
#undef TYPENAME_CONSTITERATOR
#undef TYPENAME_REFERENCE
#undef TYPENAME_CONSTREFERENCE
#undef TYPENAME_POINTER
#undef TYPENAME_DIFFERENCE
#undef TYPENAME_SIZETYPE

#if 0
// === undef globals ===
#endif

#undef IF_INT0
#undef IF_INT1

#undef MYELEM0
#undef MYELEM1
#undef MYELEM2
#undef MYELEM3
#undef MYELEM4
#undef MYELEM5
#undef MYELEM6
#undef MYELEM7
#undef MYELEM8
#undef MYELEM9

#undef THEMYELEM0
#undef THEMYELEM1
#undef THEMYELEM2
#undef THEMYELEM3
#undef THEMYELEM4
#undef THEMYELEM5
#undef THEMYELEM6
#undef THEMYELEM7
#undef THEMYELEM8
#undef THEMYELEM9

#undef NULLTYPE

#undef ELEM
#undef THEELEM

#undef THEELEMS

#undef TYPE
#undef TEMPLATE
#undef TEMPLATENULL

#undef NULLTYPES

#undef TEMPLATESPEC

#undef CTORARG

#undef NTIMES

#undef ARGTYPE

#undef OP_IF

} // namespace tupple
} // namespace boost


PREPROCESS_LATER(#endif)

#undef PREPROCESS_LATER


// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
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
#include <boost/preprocessor/repetition/repeat_from_to.hpp>


PREPROCESS_LATER(#include <boost/tupple/tupple.hpp>)
PREPROCESS_LATER(#include <boost/tupple/detail/ref_workaround.hpp>)
PREPROCESS_LATER(#include <boost/tupple/detail/iterator_tupple_traits_detail.hpp>)
PREPROCESS_LATER(#include <boost/tupple/iterator_tupple.hpp>)


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
#  define CONTTUPLE(k) container_tuple
#else
#  define TUPLE(k) tuple##k
#  define ITERTUPLE(k) iterator_tuple##k
#  define CONTTUPLE(k) container_tuple##k
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


#ifdef TUPPLE_PARTIAL_SPEC
#  define ARRAYACCESS(z,k,arg) ELEM(k,_)[arg.ELEM(k,_)]
#  define CONSTARRAYACCESS(z,k,arg) ELEM(k,_)[arg.ELEM(k,_)]
#else
#  define ARRAYACCESS(z,k,arg) detail::ref( ELEM(k,_)[arg.ELEM(k,_)] )
#  define CONSTARRAYACCESS(z,k,arg) detail::cref( ELEM(k,_)[arg.ELEM(k,_)] )
#endif

#define TYPENAME_VALUE(z,k,arg)          typename arg##k::value_type
#define TYPENAME_ITERATOR(z,k,arg)       typename arg##k::iterator
#define TYPENAME_CONSTITERATOR(z,k,arg)  typename arg##k::const_iterator

#ifdef TUPPLE_PARTIAL_SPEC
#  define TYPENAME_REFERENCE(z,k,arg)      typename arg##k::reference
#  define TYPENAME_CONSTREFERENCE(z,k,arg) typename arg##k::const_reference
#else
#  define TYPENAME_REFERENCE(z,k,arg)      detail::reference_wrapper<typename arg##k::value_type>
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


#ifdef TUPPLE_PARTIAL_SPEC
#  define CONTTRAITS(k) container_traits_tuple
#  define DETAILTRAITS2(z,k,_) T##k
#  define CONTTRAITSTYPE(k) CONTTRAITS(k)<BOOST_PP_ENUM(k,DETAILTRAITS2,_)>

#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a)>
#  define TRAITSDEFAULT(k,A)
#else
#  define CONTTRAITS(k) container_traits_tuple##k
#  define DETAILTRAITS2(z,k,_) T##k
#  define CONTTRAITSTYPE(k) CONTTRAITS(k)<BOOST_PP_ENUM(k,DETAILTRAITS2,_)>
//#  define CONTTRAITSTYPE(k) TYPE( CONTTRAITS(k),k,R )
//#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a),BOOST_PP_ENUM(k,b,_)>
#  define TRAITSTEMPLATE(k,a,b) template<BOOST_PP_ENUM_PARAMS(k,class a)>
#  define TRAITSDEFAULT(z,k,_) 
#endif


#define EMPTY(z,k,_) ELEM(k,_).empty()
#define SIZE(z,k,_)  ELEM(k,_).size()
#define BEGIN(z,k,_) ELEM(k,_).begin()
#define END(z,k,_)   ELEM(k,_).end()

#define TRAITSDEFAULT2(z,k,_) class R##k = detail::iterator_traits<typename T##k::iterator>


#ifdef TUPPLE_PARTIAL_SPEC
#define ITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_ITERATOR,R)>
#define CONSTITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_CONSTITERATOR,R)>
#else
#define ITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_ITERATOR,R)>
#define CONSTITERTUPLEDEF(k) ITERTUPLE(k)<BOOST_PP_ENUM(k,TYPENAME_CONSTITERATOR,R)>
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
TRAITSTEMPLATE( k,T,TRAITSDEFAULT )  struct CONTTUPLE(k) TEMPLATESPEC(k,r,T)   \
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
  TEMPLATE(k,S) CONTTUPLE(k)( const TYPE(CONTTUPLE(k),k,S)& rhs )         \
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
};

#ifdef TUPPLE_PARTIAL_SPEC
TEMPLATENULL(10,T) struct CONTTUPLE(10) { };
TEMPLATENULL(10,R) struct CONTTRAITS(10) { };
#endif

#ifdef TUPPLE_PARTIAL_SPEC
template<> struct CONTTUPLE(0) TEMPLATESPEC(0,10,T) : public tuple<null_type>
{
};
#else
template<class T> struct CONTTUPLE(0)
{
  typedef CONTTUPLE(0)<T> self_type;
};
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


#ifdef TUPPLE_PARTIAL_SPEC

template<class T, int N> struct n_fold_container_tuple { };

#define N_FOLD_CONTTUPLE(z,k,t)                                     \
template<class t> struct n_fold_container_tuple<t,k> {               \
  typedef CONTTUPLE(k)<BOOST_PP_ENUM(k,NTIMES,t)> type;             \
                                                                    \
  static type make( const t& arg )                                  \
  { return type(BOOST_PP_ENUM(k,NTIMES,arg)); }                     \  
};

BOOST_PP_REPEAT( 10, N_FOLD_CONTTUPLE, T )

#else

namespace detail
{
  #define N_FOLD_CONT_SELECT(z,k,t)                                   \
  template<> struct cont_select<k>                                    \
  {                                                              \
     typedef CONTTUPLE(k)<BOOST_PP_ENUM(k,NTIMES,t)> type;       \
                                                                 \
     static type make( const t& arg )                            \
     { return type(BOOST_PP_ENUM(k,NTIMES,arg)); }               \
  };


  template<class T> struct n_fold_container_helper
  {
    template<int N> struct cont_select {};
    template<> struct cont_select<0> { typedef CONTTUPLE(0)<null_type> type; };
    BOOST_PP_REPEAT_FROM_TO( 1, 10, N_FOLD_CONT_SELECT, T )
  };
} // namespace detail

template<class T, int N> struct n_fold_container_tuple
{
  typedef detail::n_fold_container_helper<T>::cont_select<N>::type type;
  
  static type make( const T& arg ) 
  { return detail::n_fold_container_helper<T>::cont_select<N>::make( arg ); }
};

#endif



#ifndef TUPPLE_PARTIAL_SPEC

  namespace detail {

    #define BASE_TYPE_SELECTOR(k) _DELAY_(container_tuple_base_type_selector,k)
    #define TEE(k) _DELAY_(T,k)

    #define BASE_TYPE_SELECT(k)                                               \
    TEMPLATE(k,T) struct BASE_TYPE_SELECTOR(k)                                \
    {                                                                         \
      typedef ::boost::mpl::if_c<                                             \
        ::boost::is_same< TEE(BOOST_PP_DEC(k)), null_type >::value,           \
          BASE_TYPE_SELECTOR(BOOST_PP_DEC(k))                                 \
           < BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(k), T ) > ::type,             \
          CONTTUPLE(k)< BOOST_PP_ENUM_PARAMS( k, T ) >                        \
      >::type type;                                                           \
    };

    // The initial case.
    template< class T0 >
    struct BASE_TYPE_SELECTOR(1)
    {
      typedef ::boost::mpl::if_c<
        ::boost::is_same< T0, null_type >::value,
          CONTTUPLE(0)< null_type >,
          CONTTUPLE(1)< T0 >
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
    TEMPLATE(k,V) container_tuple( BOOST_PP_ENUM( k, VEEVEE, _ ) ):      \
      self_type( BOOST_PP_ENUM( k, VEE, _ )  )                          \
    {}

  TEMPLATENULL(9,T) struct container_tuple:
    public detail::container_tuple_base_type_selector9< BOOST_PP_ENUM_PARAMS( 9, T ) >::type
  {
    typedef detail::container_tuple_base_type_selector9< BOOST_PP_ENUM_PARAMS( 9, T ) >::type base_type;

    container_tuple()
      : self_type()
    {}

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
      operator=( const container_tuple< BOOST_PP_ENUM_PARAMS( 9, S ) >& rhs )
    {
      //BOOST_STATIC_ASSERT(  );
      return( base_type::operator=( rhs ) );
    }
  };

#endif

} // namespace tupple
} // namespace boost


PREPROCESS_LATER(#endif)

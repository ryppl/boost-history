
// Copyright (C) 2001-2003 Roland Richter <roland@flll.jku.at>
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

#if 0
// This file is indented to generate "tupple.hpp".
// Do not include this file directly, but rather #include "boost/tupple/tupple.hpp"

// Comments which should not appear in the output are surrounded
// with #if 0 - #endif blocks throughout this file.
#endif

#define PREPROCESS_LATER(arg) arg

#ifdef TUPPLE_PARTIAL_SPEC
  PREPROCESS_LATER(#ifndef BOOST_TUPPLE_DETAIL_TUPPLE_PART_SPEC_HPP)
  PREPROCESS_LATER(#define BOOST_TUPPLE_DETAIL_TUPPLE_PART_SPEC_HPP)
#else
  PREPROCESS_LATER(#ifndef BOOST_TUPPLE_DETAIL_TUPPLE_NO_PART_SPEC_HPP)
  PREPROCESS_LATER(#define BOOST_TUPPLE_DETAIL_TUPPLE_NO_PART_SPEC_HPP)
#endif

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

PREPROCESS_LATER(#include <boost/tupple/detail/tupple_detail.hpp>)
PREPROCESS_LATER(#include <algorithm>)

#ifndef TUPPLE_PARTIAL_SPEC
  PREPROCESS_LATER(#include <boost/type_traits.hpp>)
  PREPROCESS_LATER(#include <boost/mpl/if.hpp>)
  PREPROCESS_LATER(#include <boost/static_assert.hpp>)
#endif

namespace boost {
namespace tupple {

#if 0
//
// === Global definitions used throughout the file
//

// Maximum tuple size
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

#if 0
// Depending on whether partial template specialization is available or not,
// certain names do differ.
#endif
#ifdef TUPPLE_PARTIAL_SPEC
#  define TUPLE(k) tuple
#  define MAKE_TUPLE_FUNC(k) make_tuple_function
#else
#  define TUPLE(k) tuple##k
#  define MAKE_TUPLE_FUNC(k) make_tuple_function##k
#endif

#define NULLTYPE null_type

#if 0
#  define ELEM(k,A) MYELEM##k
#  define THEELEM(k,A) THEMYELEM##k
#else
#  define ELEM(k,_) m##k
#  define THEELEM(k,_) theM##k
#endif

#define TYPE(name,k,arg) name<BOOST_PP_ENUM_PARAMS(k,arg)>
#define TEMPLATE(k,arg) template<BOOST_PP_ENUM_PARAMS(k,class arg)>
#define TEMPLATENULL(k,arg) template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(k,class arg,NULLTYPE)>

#define NULLTYPES(z,k,_) NULLTYPE

#ifdef TUPPLE_PARTIAL_SPEC
#  define TEMPLATESPEC(k,r,arg) < BOOST_PP_ENUM_PARAMS(k,arg) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,_) >
#else
#  define TEMPLATESPEC(k,r,arg)
#endif

#define CTORARG(z,k,_) ARGTYPE(k) THEELEM(k,_)

#define NTIMES(z,k,arg) arg

#define ARGTYPE(k) arg_type##k

#define OP_IF(c,op) BOOST_PP_IF(c,op,BOOST_PP_EMPTY())

#if 0
//
// === Part 1: Definition of the tuple<T0,...,Tn-1> type,
//             plus get(), make_tuple(), and tie() functions
//
#endif

#define TYPEDEF(z,k,arg) typedef arg##k type##k;
#define ARGTYPEDEF(z,k,p) typedef typename access_traits<p##k>::arg_type ARGTYPE(k);

#define GETTYPE(k) get_type##k
#define DELAY_GETTYPE(k) GETTYPE(k)
#define GETTYPEDEF(z,k,arg) typedef typename access_traits<arg##k>::non_const_type GETTYPE(k);
#define CONSTGETTYPE(k) const_get_type##k
#define DELAY_CONSTGETTYPE(k) CONSTGETTYPE(k)
#define CONSTGETTYPEDEF(z,k,arg) typedef typename access_traits<arg##k>::const_type CONSTGETTYPE(k);


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

#define TAILTYPE(k) DELAY_TAILTYPE(k)
#define DELAY_TAILTYPE(k) TUPLE(k)< IF_INT##k(DELAY_TS(BOOST_PP_INC(k)),NULLTYPE) >
#define DELAY_TS(k) BOOST_PP_ENUM_SHIFTED_PARAMS(k,T)


#define INITCTOR(z,k,_)   ELEM(k,_)(THEELEM(k,_))
#define COPYCTOR(z,k,arg) ELEM(k,_)( arg.ELEM(k,_) )
#define COPYTAIL(z,k,arg) DELAY_ELEM(BOOST_PP_INC(k),_)( arg.ELEM(k,_) )

#define ASSIGN(z,k,arg)   ELEM(k,_) = arg.ELEM(k,_);
#define SWAP(z,k,arg) std::swap( ELEM(k,_), arg.ELEM(k,_) );

#define TAILELEM(z,k,A) DELAY_ELEM(BOOST_PP_INC(k),A)

#define DELAY_ELEM(k,_)  _DELAY_ELEM(k,_)
#define _DELAY_ELEM(k,_) ELEM(k,_)

#define GET(k) get##k

#define GETCONSTMBR(z,k,_) CONSTGETTYPE(k) GET(k)() const { return ELEM(k,_); }
#define GETMBR(z,k,_)      GETTYPE(k)      GET(k)()       { return ELEM(k,_); }

#define MEMBER(z,k,_) T##k ELEM(k,_);

#define PAIRASGM()                                                       \
template <class U1, class U2>                                            \
  self_type& operator=( const std::pair<U1, U2>& p )                     \
  {                                                                      \
    ELEM(0,_) = p.first;                                                 \
    ELEM(1,_) = p.second;                                                \
    return *this;                                                        \
  }

#define ASSIGN_PAIR(b) BOOST_PP_IF( b, PAIRASGM, BOOST_PP_EMPTY )()


#define STRUCT_TUPLE(k)                                                   \
TEMPLATE(k,T) struct TUPLE(k) TEMPLATESPEC(k,BOOST_PP_SUB(MAX_N,k),T)     \
{                                                                         \
  BOOST_PP_REPEAT(k,TYPEDEF,T)                                            \
  BOOST_PP_REPEAT(k,ARGTYPEDEF,T)                                         \
                                                                          \
  BOOST_PP_REPEAT(k,GETTYPEDEF,T)                                         \
  BOOST_PP_REPEAT(k,CONSTGETTYPEDEF,T)                                    \
                                                                          \
  typedef TYPE(TUPLE(k),k,T) self_type;                                   \
  typedef TAILTYPE(BOOST_PP_DEC(k)) tail_type;                            \
                                                                          \
  TUPLE(k)() {}                                                           \
  TUPLE(k)(BOOST_PP_ENUM(k,CTORARG,_))                                    \
  : BOOST_PP_ENUM(k,INITCTOR,_) {}                                        \
                                                                          \
  TEMPLATE(k,S) TUPLE(k)( const TYPE(TUPLE(k),k,S)& rhs )                 \
  : BOOST_PP_ENUM(k,COPYCTOR,rhs) {}                                      \
                                                                          \
  TUPLE(k)( CTORARG(Z,0,A), const tail_type& tail )                       \
  : ELEM(0,A) ( THEELEM(0,A) ) BOOST_PP_COMMA_IF(BOOST_PP_DEC(k))         \
    BOOST_PP_ENUM(BOOST_PP_DEC(k),COPYTAIL,tail)                          \
  {}                                                                      \
                                                                          \
  TEMPLATE(k,S) self_type& operator=( const TYPE(TUPLE(k),k,S)& rhs ) {   \
    if( this != (self_type*)(&rhs) ) {                                    \
      BOOST_PP_REPEAT(k,ASSIGN,rhs)                                       \
    }                                                                     \
    return *this;                                                         \
  }                                                                       \
                                                                          \
  ASSIGN_PAIR( BOOST_PP_EQUAL(k,2) )                                      \
                                                                          \
  int size() const { return k; }                                          \
                                                                          \
  void swap( self_type& rhs )                                             \
  { BOOST_PP_REPEAT(k,SWAP,rhs) }                                         \
                                                                          \
  GETTYPE(0)      head()       { return ELEM(0,_); }                      \
  CONSTGETTYPE(0) head() const { return ELEM(0,_); }                      \
  tail_type tail() const                                                  \
  { return tail_type(                                                     \
     BOOST_PP_ENUM(BOOST_PP_DEC(k),TAILELEM,_) );                         \
  }                                                                       \
                                                                          \
  GETTYPE(0)      first()       { return ELEM(0,_); }                     \
  CONSTGETTYPE(0) first() const { return ELEM(0,_); }                     \
  DELAY_GETTYPE(BOOST_PP_DEC(k))      last()                              \
    { return DELAY_ELEM(BOOST_PP_DEC(k),_); }                             \
  DELAY_CONSTGETTYPE(BOOST_PP_DEC(k))  last() const                       \
    { return DELAY_ELEM(BOOST_PP_DEC(k),_); }                             \
                                                                          \
  BOOST_PP_REPEAT(k,GETCONSTMBR,_)                                        \
  BOOST_PP_REPEAT(k,GETMBR,_)                                             \
                                                                          \
  BOOST_PP_REPEAT(k,MEMBER,_)                                             \
};


#define GETFCTTYPE(k,arg) typename arg::GETTYPE(k)
#define GETFCT(k,arg) GET(k)( arg& t ) { return t.ELEM(k,A); }
#define CONSTGETFCTTYPE(k,arg) typename arg::CONSTGETTYPE(k)
#define CONSTGETFCT(k,arg) GET(k)( const arg& t ) { return t.ELEM(k,A); }

#define MAKETYPE(k,base,typ) base<BOOST_PP_ENUM(k,MAKETRAITS,typ)>
#define MAKETRAITS(z,k,typ) typename make_tuple_traits<typ##k>::type
#define MAKEARG(z,k,typ) const typ##k& THEELEM(k,A)

#define THEELEMS(z,k,A) THEELEM(k,A)

#ifdef TUPPLE_PARTIAL_SPEC
#  define REF(z,k,arg) arg##k&
#  define THETIEELEMS(z,k,arg) THEELEM(k,A)
#  define MAKETIE(k,A) TUPLE(k)<BOOST_PP_ENUM(k,REF,T)>
#else
#  define REF(z,k,arg) detail::assign_to_pointee<arg##k>
#  define THETIEELEMS(z,k,arg) detail::assign_to_pointee<arg##k>(&THEELEM(k,A))
#  define MAKETIE(k,A) make_tuple
#endif

#define TIETYPE(k,A) TUPLE(k)<BOOST_PP_ENUM(k,REF,T)>
#define TIEARG(z,k,arg) arg##k& THEELEM(k,A)




#define GET_MAKE_TIE(k)                                                   \
template<class TupleT>                                                    \
GETFCTTYPE(BOOST_PP_DEC(k),TupleT)                                        \
GETFCT(BOOST_PP_DEC(k),TupleT)                                            \
                                                                          \
template<class TupleT>                                                    \
CONSTGETFCTTYPE(BOOST_PP_DEC(k),TupleT)                                   \
CONSTGETFCT(BOOST_PP_DEC(k),TupleT)                                       \
                                                                          \
TEMPLATE(k,T) MAKETYPE(k,TUPLE(k),T)                                      \
make_tuple(BOOST_PP_ENUM(k,MAKEARG,T))                                    \
{ return MAKETYPE(k,TUPLE(k),T)( BOOST_PP_ENUM(k,THEELEMS,A) ) ;  }       \
                                                                          \
TEMPLATE(k,T) struct MAKE_TUPLE_FUNC(k) TEMPLATESPEC(k,BOOST_PP_SUB(MAX_N,k),T) \
{                                                                         \
  typedef MAKETYPE(k,TUPLE(k),T) result_type;                             \
  result_type operator()(BOOST_PP_ENUM(k,MAKEARG,T)) const                \
  { return result_type( BOOST_PP_ENUM(k,THEELEMS,A) ) ; }                 \
};                                                                        \
                                                                          \
TEMPLATE(k,T) TIETYPE(k,T) tie(BOOST_PP_ENUM(k,TIEARG,T))                 \
{ return MAKETIE(k,T)( BOOST_PP_ENUM(k,THETIEELEMS,T) ); }

#ifdef TUPPLE_PARTIAL_SPEC
TEMPLATENULL(10,T) struct TUPLE(10) { };
TEMPLATENULL(10,T) struct make_tuple_function { };
#endif


#ifdef TUPPLE_PARTIAL_SPEC
struct TUPLE(0) TEMPLATESPEC(0,10,T)
{
  int size() const { return 0; }
};
#else
template<class T> struct TUPLE(0)
{
  typedef TUPLE(0)<T> self_type;
  int size() const { return 0; }
};
#endif

STRUCT_TUPLE(1)
STRUCT_TUPLE(2)
STRUCT_TUPLE(3)
STRUCT_TUPLE(4)
STRUCT_TUPLE(5)
STRUCT_TUPLE(6)
STRUCT_TUPLE(7)
STRUCT_TUPLE(8)
STRUCT_TUPLE(9)


#ifdef TUPPLE_PARTIAL_SPEC

template<class T, int N> struct n_fold_tuple { };

#define N_FOLD_TUPLE(z,k,t)                                         \
template<class t> struct n_fold_tuple<t,k> {                        \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,t)> type;                 \
                                                                    \
  static type make( const t& arg )                                  \
  { return type(BOOST_PP_ENUM(k,NTIMES,arg)); }                     \
};

BOOST_PP_REPEAT( 10, N_FOLD_TUPLE, T )

#else

namespace detail
{
  #define N_FOLD_SELECT(z,k,t)                                  \
  template<> struct select<k>                                   \
  {                                                             \
    typedef TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,t)> type;           \
                                                                \
    static type make( const t& arg )                            \
    { return type(BOOST_PP_ENUM(k,NTIMES,arg)); }               \
  };

  template<class T> struct n_fold_helper
  {
    template<int N> struct select {};
    template<> struct select<0> { typedef TUPLE(0)<null_type> type; };
    BOOST_PP_REPEAT_FROM_TO( 1, 10, N_FOLD_SELECT, T )
  };
} // namespace detail

template<class T, int N> struct n_fold_tuple
{
  typedef detail::n_fold_helper<T>::select<N>::type type;

  static type make( const T& arg )
  { return detail::n_fold_helper<T>::select<N>::make( arg ); }
};

#endif


GET_MAKE_TIE(1)
GET_MAKE_TIE(2)
GET_MAKE_TIE(3)
GET_MAKE_TIE(4)
GET_MAKE_TIE(5)
GET_MAKE_TIE(6)
GET_MAKE_TIE(7)
GET_MAKE_TIE(8)
GET_MAKE_TIE(9)



#if 0
//
// === Part 2: Helpers for wrapping functions
//
#endif

#ifdef TUPPLE_PARTIAL_SPEC
#  define FUNC_OBJ(k) function_object
#  define PTR_FUNC(k) pointer_to_function
#else
#  define FUNC_OBJ(k) function_object##k
#  define PTR_FUNC(k) pointer_to_function##k
#endif

#ifdef TUPPLE_PARTIAL_SPEC
#  define PTRFUNC_TEMPLATESPEC(a,k,r)    < a, BOOST_PP_ENUM_PARAMS(k,T) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,A) >
#  define FUNCOBJ_TEMPLATESPEC(a,b,k,r)  < a, b, BOOST_PP_ENUM_PARAMS(k,T) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,A) >
#else
#  define PTRFUNC_TEMPLATESPEC(a,k,r)
#  define FUNCOBJ_TEMPLATESPEC(a,b,k,r)
#endif


#define FCTARG(z,k,arg) arg.ELEM(k,A)

#define FUNCTIONS(k)                                                      \
template<class F,class R,BOOST_PP_ENUM_PARAMS(k,class T)>                 \
struct FUNC_OBJ(k) FUNCOBJ_TEMPLATESPEC(F,R,k,BOOST_PP_SUB(MAX_N,k)) {    \
  typedef TYPE(TUPLE(k),k,T) argument_type;                               \
  typedef R result_type;                                                  \
                                                                          \
  FUNC_OBJ(k)() {}                                                        \
  FUNC_OBJ(k)( const F& theF )  : f( theF ) {}                            \
                                                                          \
  R operator()( const TYPE(TUPLE(k),k,T)& arg )                           \
  { return f( BOOST_PP_ENUM(k,FCTARG,arg) ); }                            \
                                                                          \
protected:                                                                \
  F f;                                                                    \
};                                                                        \
                                                                          \
template<class R,BOOST_PP_ENUM_PARAMS(k,class T)>                         \
struct PTR_FUNC(k) PTRFUNC_TEMPLATESPEC(R,k,BOOST_PP_SUB(MAX_N,k)) {      \
  typedef TYPE(TUPLE(k),k,T) argument_type;                               \
  typedef R result_type;                                                  \
                                                                          \
  PTR_FUNC(k)() {}                                                        \
  PTR_FUNC(k)( R (*theF)( BOOST_PP_ENUM_PARAMS(k,T) ) )                   \
    : f( theF ) {}                                                        \
                                                                          \
  R operator()( const TYPE(TUPLE(k),k,T)& arg )                           \
  { return f( BOOST_PP_ENUM(k,FCTARG,arg) ); }                            \
                                                                          \
protected:                                                                \
  R (*f)( BOOST_PP_ENUM_PARAMS(k,T) );                                    \
};                                                                        \
                                                                          \
template<class R,BOOST_PP_ENUM_PARAMS(k,class T)>                         \
inline PTR_FUNC(k)<R,BOOST_PP_ENUM_PARAMS(k,T)>                           \
ptr_fct( R (*theF)( BOOST_PP_ENUM_PARAMS(k,T) ) ) {                       \
  return PTR_FUNC(k)<R,BOOST_PP_ENUM_PARAMS(k,T)>( theF );                \
}

#ifdef TUPPLE_PARTIAL_SPEC
TEMPLATENULL(11,T) struct pointer_to_function { };
TEMPLATENULL(12,T) struct function_object { };
#endif

FUNCTIONS(1)
FUNCTIONS(2)
FUNCTIONS(3)
FUNCTIONS(4)
FUNCTIONS(5)
FUNCTIONS(6)
FUNCTIONS(7)
FUNCTIONS(8)
FUNCTIONS(9)

#if 0
//
// === Part 3: Relational operators
//
#endif

#define AND_EQUAL(z,k,_) OP_IF(k,&&) lhs.ELEM(k,_) == rhs.ELEM(k,_)

#define EQUAL(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator==( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs ) \
{ return( BOOST_PP_REPEAT(k,AND_EQUAL,_) ); }

inline bool operator==( const TUPLE(0)<NULLTYPE>& lhs, const TUPLE(0)<NULLTYPE>& rhs ) { return true; }

#define NOTEQ(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator!=( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs ) \
{ return !( lhs == rhs ); }

#if 0
// I doubt whether lexicographical ordering can be implemented with macros.
// It would require (?) a recursive definition, and that seems
// not possible. This version is a "work-around"
#endif

#define LESS(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator<( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs )  \
{ return( lhs.head() < rhs.head() || ( !(rhs.head() < lhs.head()) && lhs.tail() < rhs.tail() ) ); }

inline bool operator<( const TUPLE(0)<NULLTYPE>& lhs, const TUPLE(0)<NULLTYPE>& rhs ) { return false; }

#define LESSEQ(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator<=( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs ) \
{ return !(rhs < lhs) ; }

#define GREATER(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator>( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs )  \
{ return( rhs < lhs ); }

#define GREATEREQ(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator>=( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs ) \
{ return !(lhs < rhs); }

#define RELATIONAL(k) EQUAL(k) LESS(k) LESSEQ(k) GREATER(k) GREATEREQ(k)

RELATIONAL(1)
RELATIONAL(2)
RELATIONAL(3)
RELATIONAL(4)
RELATIONAL(5)
RELATIONAL(6)
RELATIONAL(7)
RELATIONAL(8)
RELATIONAL(9)

// operator!= seems to be defined within STL, sometimes,
// for instance in file stl_relops.h of SGI's STL
PREPROCESS_LATER(#ifndef TUPPLE_SKIP_UNEQUAL)
NOTEQ(1)
NOTEQ(2)
NOTEQ(3)
NOTEQ(4)
NOTEQ(5)
NOTEQ(6)
NOTEQ(7)
NOTEQ(8)
NOTEQ(9)
PREPROCESS_LATER(#endif)

#define MINMAX(k) \
template<class T> T minimal( const TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)>& t ) {  \
  return std::min( t.head(), minimal( t.tail() ) );    \
}                                                       \
template<class T> T maximal( const TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)>& t ) {  \
  return std::max( t.head(), maximal( t.tail() ) );    \
}

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


#if 0
// In case of no partial specialization, a workaround is required to use
// the name 'tuple' (instead of 'tuple2', 'tuple3' etc.) for various
// numbers of template arguments.
#endif

#ifndef TUPPLE_PARTIAL_SPEC

  namespace detail {

    #define BASE_TYPE_SELECTOR(k) _DELAY_(tuple_base_type_selector,k)
    #define TEE(k) _DELAY_(T,k)

    #define BASE_TYPE_SELECT(k)                                               \
    TEMPLATE(k,T) struct BASE_TYPE_SELECTOR(k)                                \
    {                                                                         \
      typedef ::boost::mpl::if_c<                                             \
        ::boost::is_same< TEE(BOOST_PP_DEC(k)), null_type >::value,           \
          BASE_TYPE_SELECTOR(BOOST_PP_DEC(k))                                 \
           < BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(k), T ) > ::type,             \
          TUPLE(k)< BOOST_PP_ENUM_PARAMS( k, T ) >                            \
      >::type type;                                                           \
    };

    // The initial case.
    template< class T0 >
    struct BASE_TYPE_SELECTOR(1)
    {
      typedef ::boost::mpl::if_c<
        ::boost::is_same< T0, null_type >::value,
          TUPLE(0)< null_type >,
          TUPLE(1)< T0 >
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
    TEMPLATE(k,V) tuple( BOOST_PP_ENUM( k, VEEVEE, _ ) ):               \
      self_type( BOOST_PP_ENUM( k, VEE, _ )  )                          \
    {}

  TEMPLATENULL(9,T) struct tuple:
    public detail::tuple_base_type_selector9< BOOST_PP_ENUM_PARAMS( 9, T ) >::type
  {
    typedef detail::tuple_base_type_selector9< BOOST_PP_ENUM_PARAMS( 9, T ) >::type base_type;

    tuple()
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
      operator=( const tuple< BOOST_PP_ENUM_PARAMS( 9, S ) >& rhs )
    {
      //BOOST_STATIC_ASSERT(  );
      return( base_type::operator=( rhs ) );
    }
  };

#endif

} // namespace tupple
} // namespace boost


PREPROCESS_LATER(#endif)

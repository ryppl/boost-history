
// Copyright (C) 2001,2002 Roland Richter <roland@flll.jku.at>
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
PREPROCESS_LATER(#ifndef TUPPLE_PART_SPEC_HPP)
PREPROCESS_LATER(#define TUPPLE_PART_SPEC_HPP)
#else
PREPROCESS_LATER(#ifndef TUPPLE_NO_PART_SPEC_HPP)
PREPROCESS_LATER(#define TUPPLE_NO_PART_SPEC_HPP)
#endif


#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

PREPROCESS_LATER(#include "boost/tupple/detail/tupple_detail.hpp")


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

#if 0
// Depending on whether partial template specialization is available or not,
// certain names do differ.
#endif
#ifdef TUPPLE_PARTIAL_SPEC
#  define TUPLE(k) tuple
#  define MAKE_TUPLE_FUNC(k) make_tuple_function
#  define N_FOLD_TUPLE(a,k) n_fold_tuple<a,k>
#else
#  define TUPLE(k) tuple##k
#  define MAKE_TUPLE_FUNC(k) make_tuple_function##k
#  define N_FOLD_TUPLE(a,k) n_fold_tuple##k
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
#define TEMPLATENULL(k) template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(k,class T,NULLTYPE)>

#define NULLTYPES(z,k,_) NULLTYPE

#ifdef TUPPLE_PARTIAL_SPEC
#  define TEMPLATESPEC(k,r) < BOOST_PP_ENUM_PARAMS(k,T) BOOST_PP_COMMA_IF(k) BOOST_PP_ENUM(r,NULLTYPES,A) >
#else
#  define TEMPLATESPEC(k,r)
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
#define ARGTYPEDEF(z,k,p) typedef access_traits<p##k>::arg_type ARGTYPE(k);

#define GETTYPE(k) get_type##k
#define GETTYPEDEF(z,k,arg) typedef access_traits<arg##k>::non_const_type GETTYPE(k);
#define CONSTGETTYPE(k) const_get_type##k
#define CONSTGETTYPEDEF(z,k,arg) typedef access_traits<arg##k>::const_type CONSTGETTYPE(k);


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
#define COPYCTOR(z,k,arg) ELEM(k,A)( arg.ELEM(k,A) )
#define COPYTAIL(z,k,arg) DELAY_ELEM(BOOST_PP_INC(k),A)( arg.ELEM(k,A) )

#define ASSIGN(z,k,arg)   ELEM(k,A) = arg.ELEM(k,A);
#define SWAP(z,k,arg) std::swap( ELEM(k,A), arg.ELEM(k,A) );

#define TAILELEM(z,k,A) DELAY_ELEM(BOOST_PP_INC(k),A)

#define DELAY_ELEM(k,A)  ELEM(k,A)

#define GET(k) get##k

#define GETCONSTMBR(z,k,A) CONSTGETTYPE(k) GET(k)() const { return ELEM(k,A); }
#define GETMBR(z,k,_)      GETTYPE(k)      GET(k)()       { return ELEM(k,A); }
#define MEMBER(z,k,_) T##k ELEM(k,_);


#define STRUCT_TUPLE(k)                                                   \
TEMPLATE(k,T) struct TUPLE(k) TEMPLATESPEC(k,BOOST_PP_SUB(MAX_N,k))       \
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
  int size() const { return k; }                                          \
                                                                          \
  void swap( self_type& rhs )                                             \
  { BOOST_PP_REPEAT(k,SWAP,rhs) }                                         \
                                                                          \
  get_type0       head()       { return ELEM(0,A); }                      \
  const_get_type0 head() const { return ELEM(0,A); }                      \
  tail_type tail() const                                                  \
  { return tail_type(                                                     \
     BOOST_PP_ENUM(BOOST_PP_DEC(k),TAILELEM,A) );                         \
  }                                                                       \
                                                                          \
  BOOST_PP_REPEAT(k,GETCONSTMBR,A)                                        \
  BOOST_PP_REPEAT(k,GETMBR,_)                                             \
  BOOST_PP_REPEAT(k,MEMBER,_)                                             \
};


#define GETFCTTYPE(k,arg) typename arg::GETTYPE(k)
#define GETFCT(k,arg) GET(k)( arg& t ) { return t.ELEM(k,A); }
#define CONSTGETFCTTYPE(k,arg) typename arg::CONSTGETTYPE(k)
#define CONSTGETFCT(k,arg) GET(k)( const arg& t ) { return t.ELEM(k,A); }

#define MAKETYPE(k,base,typ) base<BOOST_PP_ENUM(k,MAKETRAITS,typ)>
#define MAKETRAITS(z,k,typ) make_tuple_traits<typ##k>::type
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
                                                                          \
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
TEMPLATE(k,T) struct MAKE_TUPLE_FUNC(k) TEMPLATESPEC(k,BOOST_PP_SUB(MAX_N,k)) \
{                                                                         \
  typedef MAKETYPE(k,TUPLE(k),T) result_type;                             \
  result_type operator()(BOOST_PP_ENUM(k,MAKEARG,T)) const                \
  { return result_type( BOOST_PP_ENUM(k,THEELEMS,A) ) ; }                 \
};                                                                        \
                                                                          \
TEMPLATE(k,T) TIETYPE(k,T) tie(BOOST_PP_ENUM(k,TIEARG,T))                 \
{ return MAKETIE(k,T)( BOOST_PP_ENUM(k,THETIEELEMS,T) ); }                \
                                                                          \
template<class T> struct N_FOLD_TUPLE(T,k) {                              \
  typedef TUPLE(k)<BOOST_PP_ENUM(k,NTIMES,T)> type;                       \
};

#ifdef TUPPLE_PARTIAL_SPEC
TEMPLATENULL(10) struct TUPLE(10) { };
template<class T, int N> struct n_fold_tuple { };
TEMPLATENULL(10) struct make_tuple_function { };
#endif


#ifdef TUPPLE_PARTIAL_SPEC
struct TUPLE(0) TEMPLATESPEC(0,10)
{
  int size() const { return 0; }
};
#else
template<class T> struct TUPLE(0) { int size() const { return 0; } };
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


GET_MAKE_TIE(1)
GET_MAKE_TIE(2)
GET_MAKE_TIE(3)
GET_MAKE_TIE(4)
GET_MAKE_TIE(5)
GET_MAKE_TIE(6)
GET_MAKE_TIE(7)
GET_MAKE_TIE(8)
GET_MAKE_TIE(9)



#undef IF_INT0
#undef IF_INT1
#undef IF_INT2
#undef IF_INT3
#undef IF_INT4
#undef IF_INT5
#undef IF_INT6
#undef IF_INT7
#undef IF_INT8
#undef IF_INT9

#undef TYPEDEF
#undef ARGTYPEDEF

#undef GETTYPE
#undef GETTYPEDEF
#undef CONSTGETTYPE
#undef CONSTGETTYPEDEF

#undef TAILTYPE
#undef DELAY_TAILTYPE
#undef DELAY_TS

#undef INITCTOR
#undef COPYCTOR
#undef COPYTAIL

#undef ASSIGN
#undef SWAP

#undef TAILELEM
#undef DELAY_ELEM

#undef GET

#undef GETCONSTMBR
#undef GETMBR

#undef MEMBER

#undef GETFCTTYPE
#undef GETFCT
#undef CONSTGETFCTTYPE
#undef CONSTGETFCT

#undef MAKETYPE
#undef MAKETRAITS
#undef MAKEARG

#undef REF
#undef TIETYPE
#undef TIEARG
#undef THETIEELEMS
#undef MAKETIE

#undef STRUCT_TUPLE
#undef GET_MAKE_TIE

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
TEMPLATENULL(11) struct pointer_to_function { };
TEMPLATENULL(12) struct function_object { };
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


#undef FCTARG

#undef FUNCTIONS

#if 0
//
// === Part 3: Relational operators
//
#endif

#define AND(z,k,op) OP_IF(k,&&) lhs.ELEM(k,A) ## op ## rhs.ELEM(k,A)
#define OR(z,k,op)  OP_IF(k,||) lhs.ELEM(k,A) ## op ## rhs.ELEM(k,A)

#define EQUAL(k) \
template<BOOST_PP_ENUM_PARAMS(k,class T),BOOST_PP_ENUM_PARAMS(k,class S)> \
bool operator==( const TYPE(TUPLE(k),k,T)& lhs, const TYPE(TUPLE(k),k,S)& rhs ) \
{ return( BOOST_PP_REPEAT(k,AND,==) ); }

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
PREPROCESS_LATER(#ifndef TUPPLE_SKIP_NOT_EQUAL)
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

#undef AND
#undef OR

#undef EQUAL
#undef NOTEQ
#undef LESS
#undef LESSEQ
#undef GREATER
#undef GREATEREQ

#undef RELATIONAL

#if 0
// === undef globals ===
#endif

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

#undef MAX_N

#undef NULLTYPE

#undef ELEM
#undef THEELEM

#undef THEELEMS

#undef TYPE
#undef TEMPLATE
#undef TEMPLATENULL

#undef NULLTYPES
#undef TEMPLATESPEC
#undef PTRFUNC_TEMPLATESPEC
#undef FUNCOBJ_TEMPLATESPEC

#undef CTORARG

#undef NTIMES

#undef ARGTYPE

#undef OP_IF

} // namespace tupple
} // namespace boost


PREPROCESS_LATER(#endif)

#undef PREPROCESS_LATER

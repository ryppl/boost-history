//
// Copyright (C) 2000,2001,2002 Si-Lab b.v.b.a.
// 
// License is hereby granted to use and modify this software 
// as long as this copyright notice is retained and modifications
// are clearly marked.
// This License does not permit you to redistribute this software.
// 
//

#ifndef femtown_valuetraits_hpp
#define femtown_valuetraits_hpp

///
/// ties machine-dependent data-types to FEMTown specific data-types
///

#include <boost/limits.hpp>
#include <complex>
#include <float.h>

// namespace numerics {

  enum ValueTag { REAL, COMPLEX } ;

  template < typename T >
  struct value_traits
  {};

  template <>
  struct value_traits< float >
  {
    typedef float value_type ;

    static const float INVALID    ;
    static const float UNDEFINED  ;
    static const float I          ;
    static const float TOLERANCE  ;
    static const float EPSILON    ; 
  };

  template <>
  struct value_traits< double >
  {
    typedef double value_type ;

    static const double INVALID   ;
    static const double UNDEFINED ;
    static const double I         ;
    static const double TOLERANCE ;
    static const double EPSILON   ;
    static const ValueTag tag     ;
    static const char* name       ;
  };

  template <>
  struct value_traits< std::complex< float > >
  {
    typedef float value_type ;
    static const std::complex< float > INVALID ;
    static const std::complex< float > UNDEFINED ;
    static const std::complex< float > I        ;
    static const std::complex< float > TOLERANCE ;
    static const std::complex< float > EPSILON ; 
  };

  template <>
  struct value_traits< std::complex< double > >
  {
    typedef double value_type ;

    static const std::complex< double > INVALID   ;
    static const std::complex< double > UNDEFINED ;
    static const std::complex< double > I         ;
    static const ValueTag tag                     ;
    static const char* name                       ;
    static const std::complex< double > TOLERANCE ;
    static const std::complex< double > EPSILON   ;
  };

  template <>
  struct value_traits< int >
  {
    typedef int value_type ;

    static const int INVALID ;
    static const int UNDEFINED ;
    static const int I        ;
    static const int TOLERANCE ;
    static const int EPSILON ; 
  };

  template < typename T >
  bool eq(const T& a, const T& b, typename value_traits< T >::value_type tolerance) //  = value_traits<T>::value_type::TOLERANCE )
  { return std::abs( a - b ) < tolerance ; }

  template < typename T >
  struct is_equal
  {
    typedef typename value_traits< T >::value_type value_type ;

    is_equal(const value_type & tolerance = value_type::TOLERANCE) : tolerance_( tolerance ) {}

    bool operator()(const T& a, const T& b) const
    { return eq< T >(a,b,tolerance_) ; }

    value_type tolerance_ ;
  };
//}


#endif // femtown_valuetraits_hpp

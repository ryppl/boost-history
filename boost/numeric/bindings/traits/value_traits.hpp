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

namespace boost { namespace numeric { namespace bindings { namespace traits {

  enum ValueTag { REAL, COMPLEX } ;

  template < typename T >
  struct value_traits
  {};

  template <>
  struct value_traits< float >
  {
    typedef float value_type ;
  };

  template <>
  struct value_traits< double >
  {
    typedef double value_type ;
  };

  template <>
  struct value_traits< std::complex< float > >
  {
    typedef float value_type ;
  };

  template <>
  struct value_traits< std::complex< double > >
  {
    typedef double value_type ;
  };

  template <>
  struct value_traits< int >
  {
    typedef int value_type ;
  };

  template < typename T >
  bool eq(const T& a, const T& b, typename value_traits< T >::value_type tolerance) 
  { return std::abs( a - b ) < tolerance ; }

  template < typename T >
  struct is_equal
  {
    typedef typename value_traits< T >::value_type value_type ;

    is_equal(const value_type & tolerance) : tolerance_( tolerance ) {}

    bool operator()(const T& a, const T& b) const
    { return eq< T >(a,b,tolerance_) ; }

    value_type tolerance_ ;
  };

}}}}

#endif // femtown_valuetraits_hpp

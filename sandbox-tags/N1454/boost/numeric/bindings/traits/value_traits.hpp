//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright (C) 2002 Si-Lab b.v.b.a.

#ifndef boost_numeric_bindings_traits_value_traits_hpp
#define boost_numeric_bindings_traits_value_traits_hpp

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

}}}}

#endif // boost_numeric_bindings_traits_value_traits_hpp

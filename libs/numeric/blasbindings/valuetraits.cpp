//
// Copyright (C) 2000,2001,2002 Si-Lab b.v.b.a.
// 
// License is hereby granted to use and modify this software 
// as long as this copyright notice is retained and modifications
// are clearly marked.
// This License does not permit you to redistribute this software.
// 
//

#include <boost/numeric/blasbindings/valuetraits.hpp>
#include <boost/limits.hpp>
#include <iostream>

  const float value_traits< float >::INVALID   = std::numeric_limits< float >::max() ;
  const float value_traits< float >::UNDEFINED = std::numeric_limits< float >::max() * ( 1 - std::numeric_limits< double >::epsilon() );
  const float value_traits< float >::I         = 0 ;
  const float value_traits< float >::TOLERANCE = std::numeric_limits< float >::round_error() ;
  const float value_traits< float >::EPSILON   = std::numeric_limits< float >::epsilon() ;

  const double value_traits<double>::INVALID   = std::numeric_limits< double >::max() ;
  const double value_traits<double>::UNDEFINED = std::numeric_limits< double >::max() * ( 1 - std::numeric_limits< double >::epsilon() ) ;
  const double value_traits<double>::I         = 0 ;
  const ValueTag value_traits<double>::tag     = REAL ;
  const char* value_traits<double>::name       = "double";
  const double value_traits<double>::TOLERANCE = 1e-5 ;
  const double value_traits<double>::EPSILON   = std::numeric_limits< double >::epsilon() ;

  const std::complex<double> value_traits<std::complex<double> >::INVALID   = std::numeric_limits< std::complex<double> >::max() ;
  const std::complex<double> value_traits<std::complex<double> >::UNDEFINED = std::numeric_limits< std::complex<double> >::max() - std::complex<double>(1,1) ;
  const std::complex<double> value_traits<std::complex<double> >::I         = std::complex<double>(0,1);
  const ValueTag value_traits<std::complex<double> >::tag                   = COMPLEX ;
  const char* value_traits<std::complex<double> >::name                     = "std::complex<double>";
  const std::complex<double> value_traits<std::complex<double> >::TOLERANCE = std::complex<double>(1e-5,1e-5) ;
  const std::complex<double> value_traits<std::complex<double> >::EPSILON   = std::complex<double>( value_traits< double >::EPSILON, value_traits< double >::EPSILON ) ;

  const int value_traits< int >::INVALID   = std::numeric_limits< int >::max() ;
  const int value_traits< int >::UNDEFINED = std::numeric_limits< int >::max()-1 ;
  const int value_traits< int >::I         = 0 ;
  const int value_traits< int >::TOLERANCE = std::numeric_limits< int >::round_error() ;
  const int value_traits< int >::EPSILON   = std::numeric_limits< int >::epsilon() ;


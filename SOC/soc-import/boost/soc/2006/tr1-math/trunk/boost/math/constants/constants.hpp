//  (C) Copyright John Maddock 2005-2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_CONSTANTS_CONSTANTS_INCLUDED
#define BOOST_MATH_CONSTANTS_CONSTANTS_INCLUDED

#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>

namespace boost{ namespace math{ namespace constants{

#define BOOST_DEFINE_MATH_CONSTANT(name, x, y, exp)\
   template <class T> inline T name()\
   {\
      static const T result = ::boost::lexical_cast<T>(BOOST_STRINGIZE(BOOST_JOIN(BOOST_JOIN(x, y), BOOST_JOIN(e, exp))));\
      return result;\
   }\
   template <> inline float name<float>()\
   { return BOOST_JOIN(BOOST_JOIN(x, BOOST_JOIN(e, exp)), F); }\
   template <> inline double name<double>()\
   { return BOOST_JOIN(x, BOOST_JOIN(e, exp)); }\
   template <> inline long double name<long double>()\
   { return BOOST_JOIN(BOOST_JOIN(x, BOOST_JOIN(e, exp)), L); }

BOOST_DEFINE_MATH_CONSTANT(pi, 3.141592653589793238462643383279502884197169399375105820974944, 59230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196, 0)
BOOST_DEFINE_MATH_CONSTANT(e, 2.7182818284590452353602874713526624977572470936999595749669676, 27724076630353547594571382178525166427427466391932003059921817413596629043572900334295260595630738132328627943490763233829880753195251019011, 0)
BOOST_DEFINE_MATH_CONSTANT(half, 0.5, 0, 0)

#ifdef NTL_config__H
//
// The number of digits precision in RR can vary with each call
// so we need to recalculate these with each call:
//
template<> inline NTL::RR pi<NTL::RR>()
{ 
   NTL::RR result;
   ComputePi(result);
   return result; 
}
template<> inline NTL::RR e<NTL::RR>()
{ 
   NTL::RR result = 1;
   return exp(result); 
}
#endif


} } } // namespaces

#endif // BOOST_MATH_CONSTANTS_CONSTANTS_INCLUDED

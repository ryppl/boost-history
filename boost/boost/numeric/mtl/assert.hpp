//=======================================================================
// Copyright 2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Andrew Lumsdaine
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef MTL_ASSERT_HPP
#define MTL_ASSERT_HPP

#if defined(MTL_THROW_ON_ASSERT)
  #include <stdexcept>
  #include <string>
  #include <boost/lexical_cast.hpp>
  #define MTL_ASSERT_RANGE(expr) \
    if (!(expr)) \
      throw std::range_error( \
	__FILE__ + std::string(":") \
	+ boost::lexical_cast<std::string>(__LINE__) \
	+ ": range error" + std::string(#expr) )
  #define MTL_ASSERT_SHAPE(expr) \
    if (!(expr)) \
      throw std::invalid_argument( \
	__FILE__ + std::string(":") \
	+ boost::lexical_cast<std::string>(__LINE__) \
	+ ": shape error" + std::string(#expr) )
#elif defined(MTL_SEG_FAULT_ON_ASSERT)
  #include <string>
  #include <iostream>
  #include <boost/lexical_cast.hpp>
  #define MTL_ASSERT_RANGE(expr) \
    do if (!(expr)) { \
      std::cerr << (__FILE__ + std::string(":") \
	+ boost::lexical_cast<std::string>(__LINE__) \
	+ ": range error " + std::string(#expr) ) \
	<< std::endl; \
      int* x = 0; \
      *x = 0; \
    } while (0)
  #define MTL_ASSERT_SHAPE(expr) \
    do if (!(expr)) { \
      std::cerr << (__FILE__ + std::string(":") \
	+ boost::lexical_cast<std::string>(__LINE__) \
	+ ": shape error " + std::string(#expr) ) \
	<< std::endl; \
      int* x = 0; \
      *x = 0; \
    } while (0)
#elif defined(MTL_USE_CASSERT)
  #include <assert.h>
  #define MTL_ASSERT_RANGE(expr) assert(#expr)
  #define MTL_ASSERT_SHAPE(expr) assert(#expr)
#else
  #define MTL_ASSERT_RANGE(expr) /* nothing */
  #define MTL_ASSERT_SHAPE(expr) /* nothing */
#endif


#endif // MTL_ASSERT_HPP

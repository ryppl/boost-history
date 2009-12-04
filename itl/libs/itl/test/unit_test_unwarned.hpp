/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_UNIT_TEST_UNWARNED_HPP_JOFA_091204
#define BOOST_ITL_TEST_UNIT_TEST_UNWARNED_HPP_JOFA_091204

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4389) // boost/test/test_tools.hpp(509) : warning C4389: '==' : signed/unsigned mismatch
#endif                        

#include <boost/test/unit_test.hpp>

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif


#endif // BOOST_ITL_TEST_UNIT_TEST_UNWARNED_HPP_JOFA_091204


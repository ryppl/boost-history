// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_DATE_TIME
#define BOOST_IOFM_DATE_TIME
#  if defined(BOOST_MSVC)
#     pragma warning( push )
#     pragma warning( disable : 4666 ) // 'x' : function differs from 'x' only by calling convention
#     pragma warning( disable : 4244 ) // 'argument' : conversion from 'b' to 'b', possible loss of data
#     pragma warning( disable : 4100 ) // 'i' : unreferenced formal parameter
#     pragma warning( disable : 4511 ) // copy constructor could not be generated
#     pragma warning( disable : 4512 ) // assignment operator could not be generated
#     pragma warning( disable : 4127 ) // conditional expression is constant
#     pragma warning( disable : 4800 ) // 'void *' : forcing value to bool 'true' or 'false' (performance warning)
#  elif defined(__BORLANDC__)
      // W8066: Unreachable code in function <function>
      // W8008: Condition is always true in function <function>
#     pragma option push -w-8066 -w-8008
#  endif

#  define BOOST_DATE_TIME_NO_LIB // don't auto-link the date/time libs

#  include <boost/date_time/posix_time/posix_time.hpp>

#  if defined(BOOST_MSVC)
#     pragma warning( pop )
#  elif defined(__BORLANDC__)
#     pragma option pop
#  endif
#endif

// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(BOOST_MSVC)
# pragma warning(pop)
#else
# if BOOST_WORKAROUND(__BORLANDC__, < 0x600)
#  pragma warn .8008     // Condition always true/false.
#  pragma warn .8066     // Unreachable code.
#  pragma warn .8071     // Conversion may lose significant digits.
#  pragma warn .8072     // Suspicious pointer arithmetic.
#  pragma warn .8080     // identifier declared but never used.
# endif
#endif

//EOF
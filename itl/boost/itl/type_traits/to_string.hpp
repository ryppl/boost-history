/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
Function-templates for discrete Datatypes like int, unsigned or
    any class that provides a ++ operator c.f. iterators
-----------------------------------------------------------------------------*/

#ifndef __itl_TO_STRING_JOFA_000712_H__
#define __itl_TO_STRING_JOFA_000712_H__

#include <stdio.h>
#include <string>
#include <sstream>
#pragma warning(disable: 4996) //warns against usage of printf other format-functions

namespace boost{ namespace itl
{    

/// Static class template for the string representation of values
template <class Type>
struct to_string
{
    /** Converts all values of types to std::string that implement an operator << */
    static const std::string apply(const Type& value)
	{
		std::stringstream repr;
		repr << value;
		return repr.str();
	}
};

}} // namespace boost itl

#endif



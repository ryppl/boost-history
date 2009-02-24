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

/// static class template for the string representation of values
/**
    <b>value</b> serves as a base to for the representation
    of atomic types and classes as strings.

    The function <tt>toString</tt> is defined for all atomic (built in) types
    like <tt>int, double etc.</tt>. For types other than atomic ones namely
    classes the function template <tt>toString</tt> calls a member function
    <tt>as_string</tt> on the class-type passed to the template.

    Thereby we can implement a general string representation for template classes
    which is independent of the template parameter being an atomic or a class type.
    For every object, including atomic the string converter function toString can
    be called, provides new classes implement a memberfunction <tt>as_string</tt>
  
    @author  Joachim Faulhaber
*/
template <class Type>
struct to_string
{
    /** String converter for all types <tt>Type</tt>

        E.g.: <tt>int i=5; string s = to_string<int>::apply(i);</tt>
    */
    static const std::string apply(const Type&);
};


typedef char * CharPT;

#define RETURN_AS_STRING(format, atomicValue) \
    std::stringstream repr; \
    repr << atomicValue; \
    return repr.str();

/* Alternative macro using formated sprintf output
#define RETURN_AS_STRING(format, atomicValue) \
    char stringRepr[512]; \
    sprintf(stringRepr, format, atomicValue); \
    return stringRepr;
*/

template<> inline const std::string to_string<bool>::apply(const bool& x){ return x ? "true" : "false"; }
template<> inline const std::string to_string<char>::apply(const char& x){ RETURN_AS_STRING("%c", x); }
template<> inline const std::string to_string<short>::apply(const short& x) { RETURN_AS_STRING("%d", x); }
template<> inline const std::string to_string<int>::apply(const int& x) { RETURN_AS_STRING("%d", x);   }
template<> inline const std::string to_string<long>::apply(const long& x) { RETURN_AS_STRING("%ld", x);   }
template<> inline const std::string to_string<unsigned char>::apply(const unsigned char& x) { RETURN_AS_STRING("%uc", x);   }
template<> inline const std::string to_string<unsigned short>::apply(const unsigned short& x) { RETURN_AS_STRING("%hu", x);   }
template<> inline const std::string to_string<unsigned int>::apply(const unsigned int& x) { RETURN_AS_STRING("%u", x);   }
template<> inline const std::string to_string<unsigned long>::apply(const unsigned long& x) { RETURN_AS_STRING("%lu", x);   }
template<> inline const std::string to_string<float>::apply(const float& x) { RETURN_AS_STRING("%f", x);   }
template<> inline const std::string to_string<double>::apply(const double& x) { RETURN_AS_STRING("%lf", x);   }
template<> inline const std::string to_string<CharPT>::apply(const CharPT & x) { RETURN_AS_STRING("%s", x);   }
template<> inline const std::string to_string<std::string>::apply(const std::string& x) { return x; }

template <class Type>
inline const std::string to_string<Type>::apply(const Type& x) { return x.as_string(); }

}} // namespace boost itl

#endif



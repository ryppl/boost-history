//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright (C) 2002, 2003 Si-Lab b.v.b.a. and Toon Knapen

#ifndef femtown_fortran_hpp
#define femtown_fortran_hpp

#if defined(__GNUC__) || defined(__ICC) || defined(__sgi)
#define BIND_FORTRAN_LOWERCASE_UNDERSCORE
#elif defined(__IBMCPP__)
#define BIND_FORTRAN_LOWERCASE
#else
#error do not know how to link with fortran for the given platform
#endif

#endif // femtown_fortran_hpp

//
// Copyright (C) 2000,2001,2002 Si-Lab b.v.b.a.
// 
// License is hereby granted to use and modify this software 
// as long as this copyright notice is retained and modifications
// are clearly marked.
// This License does not permit you to redistribute this software.
// 
//

#ifndef femtown_fortran_hpp
#define femtown_fortran_hpp

#if defined(__GNUC__) || defined(__ICC)
#define BIND_FORTRAN_LOWERCASE_UNDERSCORE
#elif defined(__IBMCPP__)
#define BIND_FORTRAN_LOWERCASE
#else
#error do not know how to link with fortran for the given platform
#endif

#endif // femtown_fortran_hpp

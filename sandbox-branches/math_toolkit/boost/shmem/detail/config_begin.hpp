#include <boost/config.hpp>

#if defined(BOOST_MSVC)
#pragma warning (push)
   #if(BOOST_MSVC >= 1310)
      //
      //'function' : resolved overload was found by argument-dependent lookup
      //A function found by argument-dependent lookup (Koenig lookup) was eventually 
      //chosen by overload resolution.
      //
      //In Visual C++ .NET and earlier compilers, a different function would have 
      //been called. To pick the original function, use an explicitly qualified name.
      //
      #pragma warning (disable : 4675)
   #endif
#endif

#ifndef BOOST_SHMEM_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#if defined (BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS) || BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#define BOOST_SHMEM_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#endif
#endif


#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#  pragma warn -8026 // shut up warning "cannot inline function because ..."
#  pragma warn -8027 // shut up warning "cannot inline function because ..."
#endif

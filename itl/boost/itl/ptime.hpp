/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
itl_ptime provides adapter code for boost::posix_time::ptime.
It implements incrementation (++) decrementation (--) and a neutral element
w.r.t. addition (neutron()).
-----------------------------------------------------------------------------*/

#ifndef __itl_ptime_JOFA_080416_H__
#define __itl_ptime_JOFA_080416_H__

#include <stdio.h>
#include <string>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

//JODO Produce a better compietime error here. 
//JODO (Write Macro to reduce codereplication. Generate line info.) not needed if compile-time error is nifty.
//JODO Make it complete for all date_time classes. check portability.
#ifdef ITL_NEUTRONS_PROVIDED
#pragma message("error: No neutral element provided for type boost::posix_time::ptime")
#pragma message(".. please #include <boost/itl/ptime.hpp> PRIOR TO other itl/* classes")
"JODO: Compiletime error has to be produced, but in a nicer way."
#endif

#ifdef ITL_DIFFERENCE_TYPES_PROVIDED
#pragma message("error: No difference type provided for type boost::posix_time::ptime")
#pragma message(".. please #include <boost/itl/ptime.hpp> PRIOR TO other itl/* classes")
#endif

#ifdef ITL_SIZE_TYPES_PROVIDED
#pragma message("error: No size type provided for type boost::posix_time::ptime")
#pragma message(".. please #include <boost/itl/ptime.hpp> PRIOR TO other itl/* classes")
#endif

#define ITL_NEEDS_POSIX_TIME_PTIME_NEUTRON_VALUE
#define ITL_NEEDS_POSIX_TIME_PTIME_DIFFERENCE_TYPE
#define ITL_NEEDS_POSIX_TIME_PTIME_SIZE_TYPE

namespace boost{namespace itl
{

    // ------------------------------------------------------------------------
    boost::posix_time::ptime operator ++(boost::posix_time::ptime& x)
    {
        return x += boost::posix_time::ptime::time_duration_type::unit();
    }

    boost::posix_time::ptime operator --(boost::posix_time::ptime& x)
    {
        return x -= boost::posix_time::ptime::time_duration_type::unit();
    }

    // ------------------------------------------------------------------------
    boost::posix_time::time_duration operator ++(boost::posix_time::time_duration& x)
    {
        return x += boost::posix_time::ptime::time_duration_type::unit();
    }

    boost::posix_time::time_duration operator --(boost::posix_time::time_duration& x)
    {
        return x -= boost::posix_time::ptime::time_duration_type::unit();
    }
}} // namespace itl boost

#endif



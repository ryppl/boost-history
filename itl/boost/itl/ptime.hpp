/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
itl_ptime provides adapter code for boost::posix_time::ptime.
It implements incrementation (++) decrementation (--) and a neutral element
w.r.t. addition (neutron()).
------------------------------------------------------------------------------*/

#ifndef BOOST_ITL_PTIME_HPP_JOFA_080416
#define BOOST_ITL_PTIME_HPP_JOFA_080416

#include <stdio.h>
#include <string>
#include <sstream>
#include <boost/detail/workaround.hpp>

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4100) // boost/date_time/time.hpp(80) : warning C4100: 'as_offset' : unreferenced formal parameter
#pragma warning(disable:4244) // 'argument' : conversion from 'int' to 'unsigned short', possible loss of data
#pragma warning(disable:4702) // boost\lexical_cast.hpp(1159) : warning C4702: unreachable code
#endif                        
#include <boost/date_time/posix_time/posix_time.hpp>
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/difference.hpp>
#include <boost/itl/type_traits/size.hpp>

namespace boost{namespace itl
{
    template<> 
    inline boost::posix_time::ptime neutron<boost::posix_time::ptime>::value()
    { 
        return boost::posix_time::ptime(boost::posix_time::min_date_time); 
    }

    template<> 
    struct difference<boost::posix_time::ptime> 
    { typedef boost::posix_time::time_duration type; };  

    template<> 
    struct size<boost::posix_time::ptime> 
    { typedef boost::posix_time::time_duration type; };  

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



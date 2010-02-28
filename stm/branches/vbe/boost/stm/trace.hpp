//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_TRACE__HPP
#define BOOST_STM_TRACE__HPP

//-----------------------------------------------------------------------------
#include <iostream>
//-----------------------------------------------------------------------------
//~ << BOOST_CURRENT_FUNCTION
//#define BOOST_STM_USE_TRACE_INFO 1
#ifdef BOOST_STM_USE_TRACE_INFO
#define BOOST_STM_INFO if (false); else \
    std::cout << "*** INFO: " <<__FILE__ << "["<<__LINE__<<"]"

#else
#define BOOST_STM_INFO if (true); else \
    std::cout <<__FILE__ << "["<<__LINE__<<"]"
#endif

#define BOOST_STM_USE_TRACE_ERROR 1
#ifdef BOOST_STM_USE_TRACE_ERROR
#define BOOST_STM_ERROR if (false); else \
    std::cout << "***** ERROR: " <<__FILE__ << "["<<__LINE__<<"]"

#else
#define BOOST_STM_ERROR if (true); else \
    std::cout std::cout << "***** ERROR: " <<__FILE__ << "["<<__LINE__<<"]"
#endif

///////////////////////////////////////////////////////////////////////////////
#endif // BOOST_STM_TRACE__HPP



//  boost run_timer_ctors.cpp  -----------------------------------------------//

//  Copyright Beman Dawes 2007

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

//----------------------------------------------------------------------------//

//  These constructors are in a separate file so that this translation unit will
//  not be linked in except when one of the constructors is actually used. This
//  is important since header <iostream> is required, and it incurs the cost of
//  the standard stream objects even if they are not used.

//----------------------------------------------------------------------------//

// define BOOST_SYSTEM_SOURCE so that <boost/system/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_SYSTEM_SOURCE 

#include <boost/system/timer.hpp>
#include <iostream>

namespace boost
{
  namespace system
  {
    run_timer::run_timer( int places )
        : m_places(places), m_os(std::cout) {}

    run_timer::run_timer( const std::string & format, int places )
        : m_places(places), m_os(std::cout), m_format(format) {}

  } // namespace system
} // namespace boost

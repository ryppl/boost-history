//  boost/chrono/timer.hpp  -------------------------------------------------------//

//  Copyright Beman Dawes 2008

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_TIMER_HPP                  
#define BOOST_CHRONO_TIMER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
    
namespace boost
{
  namespace chrono
  {

//---------------------------------------------------------------------------------//
//                                 timer                                           //
//---------------------------------------------------------------------------------//

// TODO: enable commented out ec

    template <class Clock>
    class timer
    {
    public:
      typedef Clock                       clock;
      typedef typename Clock::time_point  time_point;
      typedef typename Clock::duration    duration;

      explicit timer( system::error_code & ec = system::throws )
        { start(ec); }

     ~timer() {}  // never throws

      void start( system::error_code & ec = system::throws )
        { m_start = clock::now(/*ec*/); }

      duration elapsed( system::error_code & ec = system::throws )
        { return clock::now(/*ec*/) - m_start; }

    private:
      time_point m_start;
    };

  } // namespace chrono
} // namespace boost

#endif

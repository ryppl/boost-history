//  boost timer.hpp  ---------------------------------------------------------//

//  Copyright Beman Dawes 1994-2007

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_SYSTEM_TIMER_HPP                  
#define BOOST_SYSTEM_TIMER_HPP

#include <boost/system/config.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <ostream>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace system
  {
    typedef boost::int_least64_t microsecond_t;

    struct times_t
    {
      microsecond_t wall;
      microsecond_t user;
      microsecond_t system;
    };

    //  low-level functions  -------------------------------------------------//

    BOOST_SYSTEM_DECL
    void         times( times_t & result ); // throws on error

    BOOST_SYSTEM_DECL
    error_code & times( times_t & result, error_code & ec ); // never throws

    //  timer  ---------------------------------------------------------------//

    //  unless otherwise specified, all functions throw on error

    class BOOST_SYSTEM_DECL timer
    {
    public:

      timer()                         : m_flags(m_stopped) { start(); }
      timer( const std::nothrow_t & ) : m_flags(static_cast<m_flags_t>(m_stopped
                                                  | m_nothrow)) { start(); }
     ~timer()                         {}  // never throws

      void            start();
      const times_t & stop();
      bool            stopped() const { return m_flags & m_stopped; }
      void            elapsed( times_t & result ); // does not stop()

    private:
      times_t    m_times;
      enum       m_flags_t { m_stopped=1, m_nothrow=2 };
      m_flags_t  m_flags;
    };

    //  run_timer  -----------------------------------------------------------//

    //  unless otherwise specified, all functions throw on error

    class BOOST_SYSTEM_DECL run_timer : public timer
    {
    public:

      // each constructor has two overloads to avoid an explicit default to
      // std::cout, which in turn would require including <iostream> with its
      // high associated cost even when the standard streams are not used.

      explicit run_timer( int places = 2 );
      run_timer( int places, std::ostream & os ) : m_places(places), m_os(os) {}

      explicit run_timer( const std::string & format, int places = 2 );
      run_timer( const std::string & format, int places, std::ostream & os )
         : m_places(places), m_os(os), m_format(format) {}

     ~run_timer()  // never throws
      { 
        error_code ec;
        if ( !stopped() ) report( ec );
      }

      void        report();
      error_code  report( error_code & ec ); // never throws

    private:
      int             m_places;
      std::ostream &  m_os;
      std::string     m_format;
    };

  } // namespace system
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_SYSTEM_TIMER_HPP

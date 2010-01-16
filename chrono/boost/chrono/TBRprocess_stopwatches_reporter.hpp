//  boost process_times.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2007, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_PROCESS_STOPWATCHED_REPORTER_HPP
#define BOOST_PROCESS_STOPWATCHED_REPORTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/chrono/process_clocks.hpp>
#include <boost/chrono/process_stopwatches.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <ostream>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {

    class BOOST_CHRONO_DECL process_stopwatches_reporter : public process_stopwatches
    {
      // every function making use of inlined functions of class string are not inlined to avoid DLL issues
    public:

      // each constructor form has two overloads to avoid a visible default to
      // std::cout, which in turn would require including <iostream>, with its
      // high associated cost, even when the standard streams are not used.

      explicit process_stopwatches_reporter( system::error_code & ec = system::throws );
      explicit process_stopwatches_reporter( std::ostream & os,
        system::error_code & ec = system::throws );

      explicit process_stopwatches_reporter( const std::string & format,
        system::error_code & ec = system::throws );
      explicit process_stopwatches_reporter( std::ostream & os, const std::string & format,
        system::error_code & ec = system::throws );

      explicit process_stopwatches_reporter( const std::string & format, int places,
        system::error_code & ec = system::throws );
      explicit process_stopwatches_reporter( std::ostream & os, const std::string & format,
        int places, system::error_code & ec = system::throws );

      explicit process_stopwatches_reporter( int places,
        system::error_code & ec = system::throws );
      explicit process_stopwatches_reporter( std::ostream & os, int places,
        system::error_code & ec = system::throws );

      explicit process_stopwatches_reporter( int places, const std::string & format,
        system::error_code & ec = system::throws );
      explicit process_stopwatches_reporter( std::ostream & os, int places, const std::string & format,
        system::error_code & ec = system::throws );

      ~process_stopwatches_reporter()  // never throws
      {
        system::error_code ec;
        if ( !reported() ) report( ec );
      }

      void  start( system::error_code & ec = system::throws )
      {
        m_reported = false;
        process_stopwatches::start( ec );
      }

      void  report( system::error_code & ec = system::throws );

      void  test_report( duration real_, duration user_, duration system_ );

      bool  reported() const { return m_reported; }

      static int default_places() { return m_default_places; }

    private:
      int             m_places;
      std::ostream &  m_os;
      std::string     m_format;
      bool            m_reported;

      static std::ostream &  m_cout();
      static const int m_default_places = 3;
    };

    typedef process_stopwatches_reporter run_timer; // backward comppatibility

  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_PROCESS_TIMES_HPP

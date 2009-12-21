//  boost process_times.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2007, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_PROCESS_TIMES_HPP
#define BOOST_PROCESS_TIMES_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_clocks.hpp>
#include <boost/chrono/process_stopwatches.hpp>
#include <boost/chrono/process_stopwatches_reporter.hpp>

#include <boost/system/error_code.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <ostream>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {
//--------------------------------------------------------------------------------------//
//                                  process_clocks                                       //
//--------------------------------------------------------------------------------------//
#if 0

    class BOOST_CHRONO_DECL process_clocks
    {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_clocks>    time_point;
        static const bool is_monotonic =             true;

        struct durations
        {
          process_clocks::duration                       real;    // real (i.e wall clock) time
          process_clocks::duration                       user;    // user cpu time
          process_clocks::duration                       system;  // system cpu time
        };
        struct time_points
        {
          process_clocks::time_point                       real;    // real (i.e wall clock) time
          process_clocks::time_point                       user;    // user cpu time
          process_clocks::time_point                       system;  // system cpu time
        };
    
        static void now( durations & times,
                         system::error_code & ec = system::throws );
        static void now( time_points & times,
                         system::error_code & ec = system::throws );
    };
#endif
//--------------------------------------------------------------------------------------//
//                                  process_times                                       //
//--------------------------------------------------------------------------------------//

    typedef process_clocks process_clock;
    typedef process_clocks::durations process_times;

//--------------------------------------------------------------------------------------//
//                                  process_stopwatches                                       //
//--------------------------------------------------------------------------------------//
#if 0

    class BOOST_CHRONO_DECL process_stopwatches
    // BOOST_CHRONO_DECL is required to quiet compiler warnings even though
    // process_stopwatches has no dynamically linked members, because process_stopwatches is
    // used as a base class for process_stopwatches_reporter, which does have dynamically linked members.
    {
    public:

      typedef process_clocks                          clock;
      typedef process_clocks::duration                duration;
      typedef process_clocks::time_point              time_point;

      explicit process_stopwatches( system::error_code & ec = system::throws )
      {
        start(ec);
      }

     ~process_stopwatches() {}  // never throws

      void  start( system::error_code & ec = system::throws )
      {
        process_clocks::now( m_start, ec );
      }

      void  elapsed( process_times & times, system::error_code & ec = system::throws )
      {
        process_times end;
        process_clocks::now( end, ec );
        times.real  = end.real - m_start.real;
        times.user       = end.user - m_start.user;
        times.system     = end.system - m_start.system;
      }

    protected:
      process_times   m_start;
    };
#endif
    typedef process_stopwatches process_timer; // backward comppatibility
//--------------------------------------------------------------------------------------//
//                                    process_stopwatches_reporter                                         //
//--------------------------------------------------------------------------------------//
#if 0

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
#endif
    typedef process_stopwatches_reporter run_timer; // backward comppatibility

  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_PROCESS_TIMES_HPP

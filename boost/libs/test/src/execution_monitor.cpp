//  Boost Test Library execution_monitor.cpp  --------------------------------//

//  (C) Copyright Beman Dawes and Ullrich Koethe 1995-2001. 
//  Permission to copy, use, modify, sell and distribute this software 
//  is granted provided this copyright notice appears in all copies. 
//  This software is provided "as is" without express or implied warranty, 
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

//  Revision History
//    5 Jun 01 Made SE code work with several Win32 compilers (Beman)
//    4 Jun 01 New interface to old code (was catch_exceptions.hpp)
//             to avoid use of template as requested by users (Beman)
//    4 Apr 01 Added signal handling code. (Ullrich)
//    4 Apr 01 Removed default output at end of exception handling (Ullrich)
//   26 Feb 01 Numerous changes suggested during formal review (Beman)
//   25 Jan 01 catch_exceptions.hpp code factored out of cpp_main.cpp
//   22 Jan 01 Remove test_tools dependencies to reduce coupling
//    5 Nov 00 Initial boost version (Beman Dawes)

//  Note that when testing requirements or user wishes preclude use of this
//  file as a separate compilation uses, it may be #included as a header file.

//  Header dependencies are deliberately restricted to reduce coupling to other
//  boost libraries.

#include <boost/test/execution_monitor.hpp>
#include <boost/cstdlib.hpp>  // for exit codes
#include <boost/config.hpp>   // for workarounds

#include <string>             // for string
#include <new>                // for bad_alloc
#include <typeinfo>           // for bad_cast, bad_typeid
#include <exception>          // for exception, bad_exception
#include <stdexcept>          // for std exception hierarchy

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::strlen; using ::strncat; }
# endif

// Microsoft + other compatible compilers such as Intel
#if defined(_MSC_VER)

#define BOOST_MS_SE_TRANSLATOR
#include <wtypes.h>
#include <winbase.h>
#include <excpt.h>

#elif defined(__BORLANDC__)
#include <windows.h>  // Borland 5.5.1 has its own way of doing things.

// for testing on Win32, GCC thinks it is a unix platform
// TODO: figure out how to tell it is really unix
#elif defined(__unix) && !defined(__GNUC__)

#define BOOST_UNIX_STYLE_SIGNALS
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#else
#define BOOST_NO_SIGNAL_HANDLING

#endif

namespace boost
{
  namespace detail
  {

//  error reporting machinery  -----------------------------------------------//

    static void report_error( execution_exception::error_code ec,
                              const char * msg1, const char * msg2 = "" )
    {
      static char buf[256];
      buf[0] = '\0';
      std::strncat( buf, msg1, sizeof(buf)-1 );
      std::strncat( buf, msg2, sizeof(buf)-1-std::strlen(buf) );
      throw execution_exception( ec, buf );
    }

//  Overview  ----------------------------------------------------------------//
//
//  Initial entry point:  boost::execution_monitor::execute() 
//          which calls:    boost::detail::catch_signals().

    int catch_signals( execution_monitor & em, int timeout );
    //  timeout is in seconds. 0 implies none.

# ifdef BOOST_MS_SE_TRANSLATOR
    //  this class defined per the Microsoft structured exception documentation
    class ms_se_exception
    {
      unsigned int _se_id;
    public:
      ms_se_exception( unsigned int n ) : _se_id(n){}
      ~ms_se_exception(){}
      unsigned int id() const { return _se_id; }
    };

    void ms_se_trans_func( unsigned int id, _EXCEPTION_POINTERS * exps )
      { throw ms_se_exception( id ); }
# endif  // BOOST_MS_SE_TRANSLATOR

# if defined(BOOST_MS_SE_TRANSLATOR) || defined(__BORLANDC__)
    static void report_ms_se_error( unsigned int id );
# endif

  }  // namespace detail

//  boost::execution_monitor::execute  ---------------------------------------//

  int boost::execution_monitor::execute( int timeout )
  {

# ifdef BOOST_MS_SE_TRANSLATOR
    _set_se_translator( detail::ms_se_trans_func );
# endif

    try
    {
      return detail::catch_signals( *this, timeout );
    }

    //  Catch-clause reference arguments are a bit different from function 
    //  arguments (ISO 15.3 paragraphs 18 & 19).  Apparently const isn't
    //  required.  Programmers ask for const anyhow, so we supply it.  That's
    //  easier than answering questions about non-const usage.

    catch ( const char * ex )
      { detail::report_error( execution_exception::cpp_exception_error, "C string:", ex ); }
    catch ( const std::string & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::string", ex.c_str() ); }

    //  std:: exceptions

    catch ( const std::bad_alloc & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_alloc:", ex.what() ); }

# if !defined(__BORLANDC__) || __BORLANDC__ > 0x0551
    catch ( const std::bad_cast & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_cast:", ex.what() ); }
    catch ( const std::bad_typeid & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_typeid:", ex.what() ); }
# else
    catch ( const std::bad_cast & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_cast", "" ); }
    catch ( const std::bad_typeid & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_typeid", "" ); }
# endif

    catch ( const std::bad_exception & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::bad_exception:", ex.what() ); }
    catch ( const std::domain_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::domain_error:", ex.what() ); }
    catch ( const std::invalid_argument & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::invalid_argument:", ex.what() ); }
    catch ( const std::length_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::length_error:", ex.what() ); }
    catch ( const std::out_of_range & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::out_of_range:", ex.what() ); }
    catch ( const std::range_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::range_error:", ex.what() ); }
    catch ( const std::overflow_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::overflow_error:", ex.what() ); }
    catch ( const std::underflow_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::underflow_error:", ex.what() ); }
    catch ( const std::logic_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::logic_error:", ex.what() ); }
    catch ( const std::runtime_error & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::runtime_error:", ex.what() ); }
    catch ( const std::exception & ex )
      { detail::report_error( execution_exception::cpp_exception_error, "std::exception:", ex.what() ); }

# ifdef BOOST_MS_SE_TRANSLATOR
    catch ( const detail::ms_se_exception & ex )
      { detail::report_ms_se_error( ex.id() ); }
# endif  // BOOST_MS_SE_TRANSLATOR

    catch ( const execution_exception & ) { throw; }

    catch ( ... )
      { detail::report_error( execution_exception::cpp_exception_error, "unknown type", "" ); }

    return 0;  // never reached; supplied to quiet compiler warnings
  } // execute

//  boost::detail::catch_signals()  ------------------------------------------//

  namespace detail
  {

# if defined(BOOST_UNIX_STYLE_SIGNALS)

    inline jmp_buf & unit_test_jump_buffer()
    {
        static jmp_buf unit_test_jump_buffer_;
        return unit_test_jump_buffer_;
    }

    static void unit_test_signal_handler(int sig)
    {
        longjmp(unit_test_jump_buffer(), sig);
    }

    int catch_signals( execution_monitor & em, int timeout )
    // 
    {
        volatile int sigtype;
        int result;
        execution_exception::error_code ec = execution_exception::no_error;
        const char * em;

        sigset(SIGFPE, &unit_test_signal_handler); 
        sigset(SIGTRAP, &unit_test_signal_handler); 
        sigset(SIGSEGV, &unit_test_signal_handler); 
        sigset(SIGBUS, &unit_test_signal_handler); 
        if(timeout)
        {
            sigset(SIGALRM, &unit_test_signal_handler); 
            alarm(timeout);
        }

        sigtype = setjmp(unit_test_jump_buffer());
        if(sigtype == 0)
        {
            result = em.function();
        }
        else
        {
            switch(sigtype)
            {
                case SIGALRM:
                    ec = execution_monitor::timeout_error; 
                    em = "signal: SIGALRM (timeout while executing function)";
                    break;
                case SIGTRAP:
                    ec = execution_monitor::system_error; 
                    em = "signal: SIGTRAP (perhaps integer divide by zero)";
                    break;
                case SIGFPE:
                    ec = execution_monitor::system_error; 
                    em = "signal: SIGFPE (arithmetic exception)";
                    break;
                case SIGSEGV:
                case SIGBUS:
                    ec = execution_monitor::system_fatal_error; 
                    em = "signal: memory access violation";
                    break;
                default:
                    ec = execution_monitor::system_error; 
                    em = "signal: unrecognized signal";
            }
        }
        if(timeout)
        {
            alarm(0);
            sigrelse(SIGALRM); 
        }
        sigrelse(SIGFPE); 
        sigrelse(SIGTRAP); 
        sigrelse(SIGSEGV); 
        sigrelse(SIGBUS); 

        if ( ec != execution_exception::no_error )
            { detail::report_error( ec, em ); }  // throws
        return result;
    }  // unix catch_signals

# elif defined(__BORLANDC__)

    // this works for Borland but not other Win32 compilers (which trap too many cases)
    int catch_signals( execution_monitor & em, int )
    {
      int result;

      __try { result = em.function(); }

      __except (1)
      {
        report_ms_se_error( GetExceptionCode() ); // throws
      }

      return result;
    }

# else  // default signal handler

    int catch_signals( execution_monitor & em, int )
    {
      return em.function();
    }

# endif  // choose signal handler

# if defined(BOOST_MS_SE_TRANSLATOR) || defined(__BORLANDC__)

//  report_ms_se_error() helper function  ------------------------------------//

    void report_ms_se_error( unsigned int id )
    {
      switch (id)
      {
          // cases classified as fatal_system_error
          case EXCEPTION_ACCESS_VIOLATION:
              detail::report_error( execution_exception::system_fatal_error, 
                   "memory access violation", "" );
              break;

          case EXCEPTION_ILLEGAL_INSTRUCTION:
              detail::report_error( execution_exception::system_fatal_error, 
                   "illegal instruction", "" );
              break;

          case EXCEPTION_PRIV_INSTRUCTION:
              detail::report_error( execution_exception::system_fatal_error, 
                   "privilaged instruction", "" );
              break;

          case EXCEPTION_IN_PAGE_ERROR:
              detail::report_error( execution_exception::system_fatal_error, 
                   "memory page error", "" );
              break;

          case EXCEPTION_STACK_OVERFLOW:
              detail::report_error( execution_exception::system_fatal_error, 
                   "stack overflow", "" );
              break;

          // cases classified as (non-fatal) system_trap
          case EXCEPTION_DATATYPE_MISALIGNMENT:
              detail::report_error( execution_exception::system_error, 
                  "data misalignment", "" );
              break;

          case EXCEPTION_INT_DIVIDE_BY_ZERO:
              detail::report_error( execution_exception::system_error, 
                  "integer divide by zero", "" );
              break;

          case EXCEPTION_INT_OVERFLOW:
              detail::report_error( execution_exception::system_error, 
                  "integer overflow", "" );
              break;

          case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
              detail::report_error( execution_exception::system_error, 
                  "array bounds exceeded", "" );
              break;

          case EXCEPTION_FLT_DIVIDE_BY_ZERO:
              detail::report_error( execution_exception::system_error, 
                  "floating point divide by zero", "" );
              break;

          case EXCEPTION_FLT_STACK_CHECK:
              detail::report_error( execution_exception::system_error, 
                  "floating point stack check", "" );
              break;

          case EXCEPTION_FLT_DENORMAL_OPERAND:
          case EXCEPTION_FLT_INEXACT_RESULT:
          case EXCEPTION_FLT_INVALID_OPERATION:
          case EXCEPTION_FLT_OVERFLOW:
          case EXCEPTION_FLT_UNDERFLOW:
              detail::report_error( execution_exception::system_error, 
                  "floating point error", "" );
              break;

          default:
              detail::report_error( execution_exception::system_error, 
                  "unrecognized exception or signal", "" );
      }  // switch
    }  // report_ms_se_error
#endif  // need report_ms_se_error

  } // namespace detail
} // namespace boost



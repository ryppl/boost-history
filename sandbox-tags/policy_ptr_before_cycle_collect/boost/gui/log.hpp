// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_LOG_HPP
#define BOOST_GUI_LOG_HPP

/** Message logging facilities for debugging the GUI library. */

#include <typeinfo>

#if defined(__GNUC__) && (__GNUC__ >= 3)
#  include <cxxabi.h> // abi::__cxa_demangle
#  include <malloc.h> // free

   namespace boost { namespace detail
   {
      /** Class name demangler for the GCC 3.x compiler. */

      class demangler
      {
         static int status;
         char * demangled;
         public:
            /** get the demangled name. */
            inline const char * name() const{ return demangled; }
         public:
            /** initialize the class with the demangled name of the class
              * represented by <code>ti</code>.
              */
            inline  demangler( const std::type_info & ti );
            /** cleanup for allocated memory. */
            inline ~demangler();
      };

      demangler::demangler( const std::type_info & ti ):
         demangled( abi::__cxa_demangle( ti.name(), 0, 0, &demangler::status ))
      {
      }

      demangler::~demangler()
      {
         ::free( demangled );
      }
   }}
#endif

/** Class name demangler. Usage:
  * DEMANGLE(ob).name();
  *    returns the demangled name of <code>ob</code>.
  */

#if defined(__GNUC__) && (__GNUC__ >= 3)
#  define DEMANGLE(ob) (boost::detail::demangler(typeid(ob)))
#else
#  define DEMANGLE(ob) (typeid(ob))
#endif

/** Boost.GUI tracing/logging facilities for debugging:
  * You can customize the tracing by the following defines:
  * -  BOOST_GUI_ENABLE_LOG    -- define this to enable the tracing facilities.
  * -  BOOST_GUI_LOG_TO_STREAM -- define this to redirect the trace output to
  *       a stream, e.g. std::cout.
  *
  * The library defines these macros:
  * -  BOOST_GUI_TRACE     -- send output to the trace stream, no pre-context.
  * -  BOOST_GUI_LOG(id)   -- log file, line and id information.
  * -  BOOST_GUI_LOGOB(id) -- as BOOST_GUI_LOG, but add class name and this
  *       pointer information. Used inside a class method to identify a class
  *       object.
  */

#if defined(_DEBUG) || defined(BOOST_GUI_ENABLE_LOG)
#  if defined(BOOST_GUI_LOG_TO_STREAM)
#     define BOOST_GUI_TRACE BOOST_GUI_LOG_TO_STREAM
#  else
#     include <fstream>

      namespace boost { namespace gui
      {
         extern std::ofstream log;
      }}

#     define BOOST_GUI_TRACE boost::gui::log
#  endif
#  define BOOST_GUI_LOG(id) BOOST_GUI_TRACE \
      << __FILE__ << "(" << __LINE__ << ") [" << #id << "]: "
#  define BOOST_GUI_LOGOB(id) BOOST_GUI_LOG(id) \
      << DEMANGLE(*this).name() \
      << "(" << std::hex << this << std::dec << "): "
#else
#  define BOOST_GUI_TRACE     false && std::cerr
#  define BOOST_GUI_LOG(id)   BOOST_GUI_TRACE
#  define BOOST_GUI_LOGOB(id) BOOST_GUI_TRACE
#endif

#endif

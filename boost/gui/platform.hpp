// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_PLATFORM_HPP
#define BOOST_GUI_PLATFORM_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/config.hpp>

/** Operating System detection:
 * Detect the target operating system being used as the GUI platform if
 * none have been specified. Currently supported OSs:
 * -  PLATF_WINDOWS -- The Microsoft Windows operating system (32/64-bit)
 * -  PLATF_UNIX    -- *nix operating system
 * -  PLATF_MACOS   -- Apple Macintosh operating system
 * -  PLATF_PALMOS  -- Hand-held palm device
 * -  PLATF_USER    -- User-defined platform
 */

#if defined(PLATF_NAMESPACE) && defined(PLATF_INCLUDE)
#  define PLATF_USER

// Windows
#elif defined(PLATF_WIN32) || defined(PLATF_WIN64)
#  define PLATF_WINDOWS
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define PLATF_WINDOWS
#elif defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#  define PLATF_WINDOWS
#elif defined(__CYGWIN__)
#  define PLATF_WINDOWS

// MacOS
#elif defined(PLATF_COCOA)
#  define PLATF_MACOS
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#  define PLATF_MACOS

// Unix
#elif defined(PLATF_GTK) || defined(PLATF_MOTIF) || defined(PLATF_X11)
#  define PLATF_UNIX
#elif defined(linux) || defined(__linux) || defined(__linux__)
#  define PLATF_UNIX
#elif defined(unix) || defined(__unix)
#  define PLATF_UNIX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#  define PLATF_UNIX
#elif defined(sun) || defined(__sun) || defined(__sgi) || defined(__hpux)
#  define PLATF_UNIX
#elif defined(__BEOS__)
#  define PLATF_UNIX

// Unknown
#elif defined(__IBMCPP__)
   // IBM OS/2 -- PLATF_UNIX?
#elif defined(__amigaos__)
   // AmigaOS -- PLATF_UNIX?
#endif

/** Operating System API:
 * Detect the Application Programmers Interface (API) used to develop on
 * the target operating system if none has been specified. Currently
 * supported APIs:
 * -  PLATF_WIN32  -- MS Win32 API for 32-bit Windows
 * -  PLATF_WIN64  -- MS Win64 API for 64-bit Windows
 * -  PLATF_COCOA  -- MacOS Cocoa API
 * -  PLATF_GTK    -- *nix GTK+ API
 * -  PLATF_MOTIF  -- *nix Motif/Lesstif API
 * -  PLATF_X11    -- *nix X11 API
 * -  PLATF_PALMOS -- PalmOS API (same as target OS)
 * -  PLATF_USER   -- User-defined platform
 */

#if !defined(PLATF_WIN32) && !defined(PLATF_WIN64) && \
    !defined(PLATF_COCOA) && !defined(PLATF_GTK)   && \
    !defined(PLATF_MOTIF) && !defined(PLATF_X11)   && \
    !defined(PLATF_USER)

#if defined(PLATF_WINDOWS)
#  if defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#     define PLATF_WIN64
#  else
#     define PLATF_WIN32
#  endif
#elif defined(PLATF_MACOS)
#  define PLATF_COCOA
#elif defined(PLATF_UNIX)
#  define PLATF_GTK
#endif

#endif

/** Platform Specific Layer:
 * Include the files required by the target operating system and API,
 * defining common ground for the GUI framework.
 */

#if defined(PLATF_WIN32) || defined(PLATF_WIN64)
#  include <boost/gui/win/platform.hpp>
#  define PLATF_NAMESPACE win
#elif defined(PLATF_COCOA)
#  include <boost/gui/cocoa/platform.hpp>
#  define PLATF_NAMESPACE cocoa
#elif defined(PLATF_PALMOS)
#  include <boost/gui/palmos/platform.hpp>
#  define PLATF_NAMESPACE palmos
#elif defined(PLATF_USER)
#  include PLATF_INCLUDE
#else
#  error Unknown GUI platform.
#endif

/** Platform Generic Layer:
 * Bring the platform specific code into a common area.
 */

namespace boost { namespace gui
{
   /** The boost::gui::platf namespace is an alias that refers to the
     * namespace containing the platform-specific code.
     */

   namespace platf = PLATF_NAMESPACE;

   // Bring common types into the boost::gui scope.

   using platf::char_type;
   using platf::string;
   using platf::entry_data;
   using platf::color;
   using platf::frame_style;
   using platf::id_type;
}}

#undef PLATF_NS

/** Common Entry Point:
 * An operating system usually supplies its own entry point that is different
 * from the <code>main</code> entry point used by console applications. The
 * Boost.GUI framework abstracts the entry point to a common decleration so
 * that a GUI program will be more portable.
 *
 * @param ed the arguments passed to the GUI entry point as stored in a
 *    platform-specific boost::gui::entry_data data structure.
 */

extern int gui_main( const boost::gui::entry_data & ed );

#endif

//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
//

//!
//! \file boost/process/forward_decls.hpp
//!
//! This header provides forward declarations for all public types
//! included in the library.  It is interesting to note that those types
//! that are specific to a given platform are only provided if the library
//! is being used in that same platform.
//!

#if !defined(BOOST_PROCESS_FORWARD_DECLS_HPP)
/** \cond */
#define BOOST_PROCESS_FORWARD_DECLS_HPP
/** \endcond */

#include <boost/process/config.hpp>

/** \cond */
namespace boost {
namespace process {

// ------------------------------------------------------------------------

template< class Command_Line >
class basic_pipeline;

class child;

class children;

class command_line;

class launcher;

class pistream;

class postream;

class status;

typedef basic_pipeline< command_line > pipeline;

#if defined(BOOST_PROCESS_POSIX_API)
class posix_child;

class posix_launcher;
#elif defined(BOOST_PROCESS_WIN32_API)
class win32_child;

class win32_launcher;
#else
#   error "Unsupported platform."
#endif

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost
/** \endcond */

#endif // !defined(BOOST_PROCESS_FORWARD_DECLS_HPP)

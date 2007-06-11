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
//! \file boost/process/stream_behavior.hpp
//!
//! Includes the declaration of the stream_behavior enumeration.
//!

#if !defined(BOOST_PROCESS_STREAM_BEHAVIOR_HPP)
/** \cond */
#define BOOST_PROCESS_STREAM_BEHAVIOR_HPP
/** \endcond */

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Describes the possible states for a communication stream.
//!
//! Describes the possible states for a child's communication stream.
//!
enum stream_behavior {
    //!
    //! The stream is closed and hence the child process will not be
    //! allowed to access it.
    //!
    close_stream,

    //!
    //! The child inherits the parent's handle for the data flow, thus
    //! effectively sharing it between the two processes.
    //!
    inherit_stream,

    //!
    //! The child is connected to the parent so that they can send and
    //! receive data through the stream.
    //!
    redirect_stream,

    //!
    //! The child's stream is redirected to a null device so that its
    //! output is lost.  It is important to see that this is different to
    //! close_stream because the child is still able to write data.  If we
    //! closed, e.g. stdout, it's most likely that the child couldn't work
    //! at all!
    //!
    silent_stream
};

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_STREAM_BEHAVIOR_HPP)

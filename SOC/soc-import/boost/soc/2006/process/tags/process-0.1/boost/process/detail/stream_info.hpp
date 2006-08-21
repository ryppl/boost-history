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
//! \file boost/process/detail/stream_info.hpp
//!
//! Provides the definition of the stream_info structure.
//!

#if !defined(BOOST_PROCESS_DETAIL_STREAM_INFO_HPP)
/** \cond */
#define BOOST_PROCESS_DETAIL_STREAM_INFO_HPP
/** \endcond */

#include <string>

#include <boost/optional.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/pipe.hpp>

namespace boost {
namespace process {
namespace detail {

// ------------------------------------------------------------------------

//!
//! \brief Configuration data for a file descriptor.
//!
//! This convenience structure provides a compact way to pass information
//! around on how to configure a file descriptor.  It is used in
//! conjunction with the info_map map to create an unidirectional
//! association between file descriptors and their configuration details.
//!
struct stream_info
{
    enum type { close, inherit, usefile, usehandle, usepipe } m_type;

    // Valid when m_type == usefile.
    std::string m_file;

    // Valid when m_type == usehandle.
    file_handle m_handle;

    // Valid when m_type == usepipe.
    boost::optional< pipe > m_pipe;
};

// ------------------------------------------------------------------------

} // namespace detail
} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_DETAIL_STREAM_INFO_HPP)

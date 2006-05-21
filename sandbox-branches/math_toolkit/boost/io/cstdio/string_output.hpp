//  Boost io/cstdio/string_output.hpp header file  ---------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

/** \file
    \brief C-like string output for C++ output streams

    Contains analog of the \c std::fputs function, using C++ strings and output
    streams instead of null-terminated strings and \c std::FILE pointers.
 */

#ifndef BOOST_IO_CSTDIO_STRING_OUTPUT_HPP
#define BOOST_IO_CSTDIO_STRING_OUTPUT_HPP

#include <ostream>  // for std::basic_ostream
#include <string>   // for std::basic_string


namespace boost
{
namespace io
{
namespace cstdio
{


//  Forward declarations  ----------------------------------------------------//

//! Writes a string to an output stream
template < typename Ch, class Tr, class Al >
    bool  iputs( ::std::basic_string<Ch, Tr, Al> const &s,
     ::std::basic_ostream<Ch, Tr> &o );


}  // namespace cstdio
}  // namespace io
}  // namespace boost


//  String writing function definition  --------------------------------------//

/** Sends a C++ string to an output stream, unformatted.  It works by calling
    <code><var>o</var>.write( <var>s</var>.data(), <var>s</var>.length()
    )</code>.

    \param s  The string to be written.
    \param o  The output stream to perform the writing.

    \retval true   If \p o is in a good state after the write.
    \retval false  If \p o is not in a good state after the write.
 */
template < typename Ch, class Tr, class Al >
inline
bool
boost::io::cstdio::iputs
(
    std::basic_string<Ch, Tr, Al> const &  s,
    std::basic_ostream<Ch, Tr> &           o
)
{
    return o.write( s.data(), static_cast<std::streamsize>(s.length()) ).good();
}


#endif  // BOOST_IO_CSTDIO_STRING_OUTPUT_HPP

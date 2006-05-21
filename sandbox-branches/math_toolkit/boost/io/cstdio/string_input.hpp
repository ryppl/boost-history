//  Boost io/cstdio/string_input.hpp header file  ----------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

/** \file
    \brief C-like string input for C++ input streams

    Contains analog of the \c std::fgets function, using C++ strings and input
    streams instead of null-terminated strings and \c std::FILE pointers.
 */

#ifndef BOOST_IO_CSTDIO_STRING_INPUT_HPP
#define BOOST_IO_CSTDIO_STRING_INPUT_HPP

#include <istream>  // for std::basic_istream
#include <string>   // for std::basic_string, std::getline


namespace boost
{
namespace io
{
namespace cstdio
{


//  Forward declarations  ----------------------------------------------------//

//! Reads a string from an input stream
template < typename Ch, class Tr, class Al >
    bool  igets( ::std::basic_string<Ch, Tr, Al> &s,
     ::std::basic_istream<Ch, Tr> &i );


}  // namespace cstdio
}  // namespace io
}  // namespace boost


//  String reading function definition  --------------------------------------//

/** Receives a C++ string to an input stream, unformatted.  It works by calling
    <code>std::getline( <var>i</var>, <var>s</var> )</code>, so it will stop
    early if a newline character is encountered (which is extracted but not
    stored).  Otherwise, the reading stops only if the stream ends or the string
    size limit of <code><var>s</var>.max_size()</code> characters is reached.

    \param s  The string to store the characters read.
    \param i  The input stream to perform the reading.

    \retval true   If \p i is not in a failed state after the write.
    \retval false  If \p i is in a failed state after the write.
 */
template < typename Ch, class Tr, class Al >
inline
bool
boost::io::cstdio::igets
(
    std::basic_string<Ch, Tr, Al> &  s,
    std::basic_istream<Ch, Tr> &     i
)
{
    return not std::getline( i, s ).fail();
}


#endif  // BOOST_IO_CSTDIO_STRING_INPUT_HPP

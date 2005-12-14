//  Boost io/cstdio/simple_output.hpp header file  ---------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

/** \file
    \brief C-like character and pointer-string output for C++ output streams

    Contains analogs of the \c std::fputc and \c std::fputs functions, using
    C++ output streams instead of \c std::FILE* as the sink.
 */

#ifndef BOOST_IO_CSTDIO_SIMPLE_OUTPUT_HPP
#define BOOST_IO_CSTDIO_SIMPLE_OUTPUT_HPP

#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{
namespace cstdio
{


//  Forward declarations  ----------------------------------------------------//

//! Writes a single character to an output stream
template < typename Ch, class Tr >
    bool  iputc( Ch c, ::std::basic_ostream<Ch, Tr> &o );

//! Writes a null-terminated string to an output stream
template < typename Ch, class Tr >
    bool  iputs( Ch const *s, ::std::basic_ostream<Ch, Tr> &o );


}  // namespace cstdio
}  // namespace io
}  // namespace boost


//  Single-character writing function definition  ----------------------------//

/** Sends a single character to an output stream, unformatted.  It works by
    calling <code><var>o</var>.put( <var>c</var> )</code>.

    \param c  The character to be written.
    \param o  The output stream to perform the writing.

    \retval true   If \p o is in a good state after the write.
    \retval false  If \p o is not in a good state after the write.
 */
template < typename Ch, class Tr >
inline
bool
boost::io::cstdio::iputc
(
    Ch                            c,
    std::basic_ostream<Ch, Tr> &  o
)
{
    return o.put( c ).good();
}


//  C-string writing function definition  ------------------------------------//

/** Sends a null-terminated string to an output stream, unformatted.  It works
    by calling <code><var>o</var>.write( <var>s</var>, Tr::length(<var>s</var>)
    )</code>.

    \param s  The string to be written.
    \param o  The output stream to perform the writing.

    \retval true   If \p o is in a good state after the write.
    \retval false  If \p o is not in a good state after the write.
 */
template < typename Ch, class Tr >
inline
bool
boost::io::cstdio::iputs
(
    Ch const *                    s,
    std::basic_ostream<Ch, Tr> &  o
)
{
    return o.write( s, static_cast<std::streamsize>(Tr::length( s )) ).good();
}


#endif  // BOOST_IO_CSTDIO_SIMPLE_OUTPUT_HPP

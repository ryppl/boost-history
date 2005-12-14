//  Boost io/cstdio/simple_input.hpp header file  ----------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

/** \file
    \brief C-like character and pointer-string input for C++ input streams

    Contains analogs of the \c std::fgetc and \c std::fgets functions, using
    C++ input streams instead of \c std::FILE* as the source.  An analog of
    \c std::ungetc is provided too.
 */

#ifndef BOOST_IO_CSTDIO_SIMPLE_INPUT_HPP
#define BOOST_IO_CSTDIO_SIMPLE_INPUT_HPP

#include <ios>      // for std::streamsize
#include <istream>  // for std::basic_istream


namespace boost
{
namespace io
{
namespace cstdio
{


//  Forward declarations  ----------------------------------------------------//

//! Reads a single character from an input stream
template < typename Ch, class Tr >
    typename Tr::int_type  igetc( ::std::basic_istream<Ch, Tr> &i );

//! Unreads a single character back into an input stream
template < typename Ch, class Tr >
    bool  iungetc( Ch c, ::std::basic_istream<Ch, Tr> &i );

//! Reads a string of characters from an input stream
template < typename Ch, class Tr >
    bool  igets( Ch *s, ::std::streamsize n, ::std::basic_istream<Ch, Tr> &i );


}  // namespace cstdio
}  // namespace io
}  // namespace boost


//  Single-character reading function definitions  ---------------------------//

/** Receives a single character from an input stream, unformatted.  It works by
    calling <code><var>i</var>.get()</code>.

    \param i  The input stream to perform the reading.

    \return  For a successful read, a copy of the read character (expanded by
             <code>Tr::to_int_type</code>), <code>Tr::eof()</code> otherwise.
 */
template < typename Ch, class Tr >
inline
typename Tr::int_type
boost::io::cstdio::igetc
(
    std::basic_istream<Ch, Tr> &  i
)
{
    return i.get();
}

/** Places a single character back into an input stream.  It works by calling
    <code><var>i</var>.putback( <var>c</var> )</code>.

    \param c  The character to be placed back.
    \param i  The input stream to be affected.

    \retval true   If \p i is in a good state after the put-back.
    \retval false  If \p i is not in a good state after the put-back.
 */
template < typename Ch, class Tr >
inline
bool
boost::io::cstdio::iungetc
(
    Ch                            c,
    std::basic_istream<Ch, Tr> &  i
)
{
    return i.putback( c ).good();
}


//  C-string reading function definition  ------------------------------------//

/** Receives a null-terminated string from an input stream, unformatted.  It
    works by calling <code><var>i</var>.get( <var>s</var>, <var>n</var> )<code>,
    so it will append a null terminator after the copied characters.  If \c get
    ended by encountering a newline character, that character will be extracted
    from the stream and copied into the string before the null terminator.  (If
    newline handling is done, it has an effect of resetting \c gcount to 1.)

    \param s  The buffer to store the characters read.
    \param n  The size of the buffer.  The buffer size has to account for the
              null terminator, so the maximum characters read is one less.
    \param i  The input stream to perform the reading.

    \retval true   If \p i is not in a failed state after the read.
    \retval false  If \p i is in a failed state after the read.
 */
template < typename Ch, class Tr >
bool
boost::io::cstdio::igets
(
    Ch *                          s,
    std::streamsize               n,
    std::basic_istream<Ch, Tr> &  i
)
{
    bool                   result = not i.get( s, n ).fail();
    std::streamsize const  count = i.gcount();

    // Make sure that the reading ended only because of a newline
    if ( result && (count < n - 1) && !i.eof() )
    {
        result = not i.ignore().fail();           // extract the newline
        Tr::move( s + count + 1, s + count, 1 );  // move the null terminator
        Tr::assign( s[count], i.widen('\n') );    // insert a newline
    }

    return result;
}


#endif  // BOOST_IO_CSTDIO_SIMPLE_INPUT_HPP

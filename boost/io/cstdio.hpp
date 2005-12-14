//  Boost io/cstdio.hpp header file  -----------------------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

/** \file
    \brief C and C++ I/O facilities in terms of the other

    Express the C library's facilities for I/O in terms of C++ concepts.
    Adapt the C++ standard I/O facilities into functions like the C library.
 */

/** \dir cstdio
    \brief Components for \<boost/io/cstdio.hpp\>

    The header files within this directory section the various facilities
    into separate closures.  Items within those header files are in the
    namespace <code>boost::io::cstdio</code>.  The grouping header file
    \<boost/io/cstdio.hpp\> places the item names within namespace
    <code>boost::io</code> with <code>using</code> directives.
 */

#ifndef BOOST_IO_CSTDIO_HPP
#define BOOST_IO_CSTDIO_HPP

// Group #includes
#include <boost/io/cstdio/simple_output.hpp>
#include <boost/io/cstdio/string_output.hpp>
#include <boost/io/cstdio/simple_input.hpp>
#include <boost/io/cstdio/string_input.hpp>


namespace boost
{
namespace io
{


// types and macros (constants and variables)
// FILE, fpos_t, _IOFBF, _IOLBF, _IONBF, BUFSIZ, EOF, FOPEN_MAX, FILENAME_MAX,
// L_tmpnam, SEEK_CUR, SEEK_END, SEEK_SET, TMP_MAX, stderr, stdin, stdout

// wide character input functions
// fwscanf, wscanf, vfwscanf, vwscanf

// wide character output functions
// _removed_, fwprintf, wprintf, vfwprintf, vwprintf

// wide character input/output functions
// _removed_, wide character input functions, wide character output functions

// byte input/output functions
// fprintf, fread, fscanf, fwrite, printf, scanf, vfprintf, vfscanf, vprintf,
// vscanf, _removed_

// operations on files
// remove, rename, tmpfile, tmpnam

// file access functions
// fclose, fflush, fopen, freopen, setbuf, setvbuf

// formatted input/output functions
// fprintf, fscanf, printf, scanf, snprintf, sprintf, sscanf, vfprintf,
// vfscanf, vprintf, vscanf, vsnprintf, vsprintf, vsscanf

// FINISHED: character input/output functions

// direct input/output functions
// fread, fwrite

// file positioning functions
// fgetpos, fseek, fsetpos, ftell, rewind

// error handling functions
// clearerr, feof, ferror, perror


//  Re-declarations of #included items into a higher namespace  --------------//

// simple_output.hpp and string_output.hpp
using cstdio::iputc;
using cstdio::iputs;

// simple_input.hpp and string_input.hpp
using cstdio::igetc;
using cstdio::iungetc;
using cstdio::igets;


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_CSTDIO_HPP

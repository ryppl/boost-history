//  Boost io/iomanip_in.hpp header file  -------------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_IN_HPP
#define BOOST_IO_IOMANIP_IN_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/io/iomanip_repeat.hpp>  // for boost:io:repeat_ch, repeat_char

#include <ios>      // for std::streamsize
#include <istream>  // for std::basic_istream


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

repeat_char  skip_lines( std::streamsize count, bool final_sync = false );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  skipl( std::basic_istream<Ch, Tr> &is );


//  I/O-manipulator function (template) definitions  -------------------------//

inline
repeat_char
skip_lines
(
    std::streamsize  count,
    bool             final_sync  // = false
)
{
    return repeat_char( '\n', count, final_sync );
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
skipl
(
    std::basic_istream<Ch, Tr> &  is
)
{
    return is >> skip_lines( 1 );
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_IN_HPP

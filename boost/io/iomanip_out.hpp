//  Boost io/iomanip_out.hpp header file  ------------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_OUT_HPP
#define BOOST_IO_IOMANIP_OUT_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/io/iomanip_repeat.hpp>  // for boost:io:repeat_ch, repeat_char

#include <ios>      // for std::streamsize
#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

repeat_char  new_lines( std::streamsize count, bool final_flush = false );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  newl( std::basic_ostream<Ch, Tr> &os );


//  I/O-manipulator function (template) definitions  -------------------------//

inline
repeat_char
new_lines
(
    std::streamsize  count,
    bool             final_flush  // = false
)
{
    return repeat_char( '\n', count, final_flush );
}

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
newl
(
    std::basic_ostream<Ch, Tr> &  os
)
{
    return os << new_lines( 1 );
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_OUT_HPP

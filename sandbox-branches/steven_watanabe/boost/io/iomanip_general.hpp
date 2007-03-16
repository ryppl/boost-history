//  Boost io/iomanip_general.hpp header file  --------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_GENERAL_HPP
#define BOOST_IO_IOMANIP_GENERAL_HPP

#include <boost/io_fwd.hpp>  // self include

#include <ios>  // for std::basic_ios and std::ios_base


namespace boost
{
namespace io
{


//  I/O-manipulator function template declaration  ---------------------------//

template < typename Ch, class Tr >
    std::basic_ios<Ch, Tr> &  resetios( std::basic_ios<Ch, Tr> &s );


//  I/O-manipulator function template definition  ----------------------------//

template < typename Ch, class Tr >
inline
std::basic_ios<Ch, Tr> &
resetios
(
    std::basic_ios<Ch, Tr> &  s
)
{
    s.fill( s.widen(' ') );
    s.precision( 6 );
    s.width( 0 );
    s.flags( std::ios_base::skipws | std::ios_base::dec );

    return s;
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_GENERAL_HPP

//  Boost io/iomanip.hpp header file  ----------------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_HPP
#define BOOST_IO_IOMANIP_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/limits.hpp>  // for std::numeric_limits

#include <ios>      // for std::basic_ios, std::ios_base
#include <istream>  // for std::basic_istream
#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{


//  I/O-manipulator function template declarations  --------------------------//

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  newl( std::basic_ostream<Ch, Tr> &os );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  skipl( std::basic_istream<Ch, Tr> &is );

template < typename Ch, class Tr >
    std::basic_ios<Ch, Tr> &  resetios( std::basic_ios<Ch, Tr> &s );


//  I/O-manipulator function template definitions  ---------------------------//

template < typename Ch, class Tr >
std::basic_ostream<Ch, Tr> &
newl
(
    std::basic_ostream<Ch, Tr> &  os
)
{
    return os.put( os.widen('\n') );
}

template < typename Ch, class Tr >
std::basic_istream<Ch, Tr> &
skipl
(
    std::basic_istream<Ch, Tr> &  is
)
{
    return is.ignore( std::numeric_limits<std::streamsize>::max(),
     Tr::to_int_type(is.widen( '\n' )) );
}

template < typename Ch, class Tr >
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


#endif  // BOOST_IO_IOMANIP_HPP

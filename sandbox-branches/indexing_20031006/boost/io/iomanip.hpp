//  Boost io/iomanip.hpp header file  ----------------------------------------//

//  (C) Copyright Daryle Walker 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_IO_IOMANIP_HPP
#define BOOST_IO_IOMANIP_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/limits.hpp>  // for std::numeric_limits

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


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_HPP

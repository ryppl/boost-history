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

template < typename Ch = char >
    class multi_basic_skipper;

class multi_skipper;

multi_skipper  multi_skipl( std::streamsize count, bool final_sync = false );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  skipl( std::basic_istream<Ch, Tr> &is );


//  I/O-manipulator operator function declarations  --------------------------//

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &is,
     multi_basic_skipper<Ch> const &s );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &is,
     multi_skipper const &s );


//  I/O-manipulator object class (template) declarations  --------------------//

template < typename Ch >
class multi_basic_skipper
    : private repeat_ch<Ch>
{
    typedef repeat_ch<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    multi_basic_skipper( char_type c, std::streamsize count,
     bool final_sync = false );

    // Accessors
    using base_type::repeated_char;
    using base_type::repeat_count;
    using base_type::will_synchronize_afterwards;

    // Operator
    template < class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::multi_basic_skipper

class multi_skipper
    : private repeat_char
{
    typedef repeat_char  base_type;

public:
    // Lifetime management
    multi_skipper( char c, ::std::streamsize count, bool final_sync = false );

    // Accessors
    using base_type::repeated_char;
    using base_type::repeat_count;
    using base_type::will_synchronize_afterwards;

    // Operator
    template < typename Ch, class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::multi_skipper


//  I/O-manipulator function (template) definitions  -------------------------//

inline
multi_skipper
multi_skipl
(
    std::streamsize  count,
    bool             final_sync  // = false
)
{
    return multi_skipper( '\n', count, final_sync );
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
skipl
(
    std::basic_istream<Ch, Tr> &  is
)
{
    return is >> multi_skipl( 1 );
}


//  I/O-manipulator object class (template) member function definitions  -----//

template < typename Ch >
inline
multi_basic_skipper<Ch>::multi_basic_skipper
(
    char_type        c,
    std::streamsize  count,
    bool             final_sync  // = false
)
    : base_type( c, count, final_sync )
{
}

template < typename Ch >
template < class Tr >
inline
void
multi_basic_skipper<Ch>::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    this->base_type::operator ()( is );
}

inline
multi_skipper::multi_skipper
(
    char             c,
    std::streamsize  count,
    bool             final_sync  // = false
)
    : base_type( c, count, final_sync )
{
}

template < typename Ch, class Tr >
inline
void
multi_skipper::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    this->base_type::operator ()( is );
}


//  I/O-manipulator operator function definitions  ---------------------------//

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &     is,
    multi_basic_skipper<Ch> const &  s
)
{
    return s( is ), is;
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  is,
    multi_skipper const &         s
)
{
    return s( is ), is;
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_IN_HPP

//  Boost io/iomanip_in.hpp header file  -------------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_IN_HPP
#define BOOST_IO_IOMANIP_IN_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/detail/repeat_char_base.hpp>

#include <boost/limits.hpp>  // for std::numeric_limits

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
    : public detail::repeated_character_streamer_base<Ch>
{
    typedef detail::repeated_character_streamer_base<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    multi_basic_skipper( char_type c, std::streamsize count,
     bool final_sync = false );

    // Operator
    template < class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::multi_basic_skipper

class multi_skipper
    : public detail::repeated_character_streamer_base<>
{
    typedef detail::repeated_character_streamer_base<>  base_type;

public:
    // Lifetime management
    multi_skipper( char c, ::std::streamsize count, bool final_sync = false );

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
    typename Tr::int_type const  ci = Tr::to_int_type( this->repeated_char() );

    for ( std::streamsize i = this->repeat_count() ; (i > 0) && is ; --i )
    {
        is.ignore( std::numeric_limits<std::streamsize>::max(), ci );
    }

    if ( this->will_synchronize_afterwards() && is )
    {
        is.sync();
    }
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
    is >> multi_basic_skipper<Ch>( is.widen(this->repeated_char()),
     this->repeat_count(), this->will_synchronize_afterwards() );
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

//  Boost io/iomanip_out.hpp header file  ------------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_OUT_HPP
#define BOOST_IO_IOMANIP_OUT_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/detail/repeat_char_base.hpp>

#include <ios>      // for std::streamsize
#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < typename Ch = char >
    class multi_basic_newer;

class multi_newer;

multi_newer    multi_newl( std::streamsize count, bool final_flush = false );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  newl( std::basic_ostream<Ch, Tr> &os );


//  I/O-manipulator operator function declarations  --------------------------//

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &os,
     multi_basic_newer<Ch> const &n );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &os,
     multi_newer const &n );


//  I/O-manipulator object class (template) declarations  --------------------//

template < typename Ch >
class multi_basic_newer
    : public detail::repeated_character_streamer_base<Ch>
{
    typedef detail::repeated_character_streamer_base<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    multi_basic_newer( char_type c, std::streamsize count,
     bool final_flush = false );

    // Operator
    template < class Tr >
    void  operator ()( ::std::basic_ostream<Ch, Tr> &os ) const;

};  // boost::io::multi_basic_newer

class multi_newer
    : public detail::repeated_character_streamer_base<>
{
    typedef detail::repeated_character_streamer_base<>  base_type;

public:
    // Lifetime management
    multi_newer( char c, ::std::streamsize count, bool final_flush = false );

    // Operator
    template < typename Ch, class Tr >
    void  operator ()( ::std::basic_ostream<Ch, Tr> &os ) const;

};  // boost::io::multi_newer


//  I/O-manipulator function (template) definitions  -------------------------//

inline
multi_newer
multi_newl
(
    std::streamsize  count,
    bool             final_flush  // = false
)
{
    return multi_newer( '\n', count, final_flush );
}

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
newl
(
    std::basic_ostream<Ch, Tr> &  os
)
{
    return os << multi_newl( 1 );
}


//  I/O-manipulator object class (template) member function definitions  -----//

template < typename Ch >
inline
multi_basic_newer<Ch>::multi_basic_newer
(
    char_type        c,
    std::streamsize  count,
    bool             final_flush  // = false
)
    : base_type( c, count, final_flush )
{
}

template < typename Ch >
template < class Tr >
inline
void
multi_basic_newer<Ch>::operator ()
(
    std::basic_ostream<Ch, Tr> &  os
) const
{
    char_type const  cc = this->repeated_char();

    for ( std::streamsize i = this->repeat_count() ; (i > 0) && os ; --i )
    {
        os.put( cc );
    }

    if ( this->will_synchronize_afterwards() && os )
    {
        os.flush();
    }
}

inline
multi_newer::multi_newer
(
    char             c,
    std::streamsize  count,
    bool             final_flush  // = false
)
    : base_type( c, count, final_flush )
{
}

template < typename Ch, class Tr >
inline
void
multi_newer::operator ()
(
    std::basic_ostream<Ch, Tr> &  os
) const
{
    os << multi_basic_newer<Ch>( os.widen(this->repeated_char()),
     this->repeat_count(), this->will_synchronize_afterwards() );
}


//  I/O-manipulator operator function definitions  ---------------------------//

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
operator <<
(
    std::basic_ostream<Ch, Tr> &   os,
    multi_basic_newer<Ch> const &  n
)
{
    return n( os ), os;
}

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
operator <<
(
    std::basic_ostream<Ch, Tr> &  os,
    multi_newer const &           n
)
{
    return n( os ), os;
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_OUT_HPP

//  Boost io/iomanip.hpp header file  ----------------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_HPP
#define BOOST_IO_IOMANIP_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/limits.hpp>  // for std::numeric_limits

#include <ios>      // for std::streamsize, std::basic_ios, std::ios_base
#include <istream>  // for std::basic_istream
#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{


//  I/O-manipulator function (template) declarations  ------------------------//

multi_newer    multi_newl( std::streamsize count, bool final_flush = false );

multi_skipper  multi_skipl( std::streamsize count, bool final_sync = false );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  newl( std::basic_ostream<Ch, Tr> &os );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  skipl( std::basic_istream<Ch, Tr> &is );

template < typename Ch, class Tr >
    std::basic_ios<Ch, Tr> &      resetios( std::basic_ios<Ch, Tr> &s );


//  I/O-manipulator operator function declarations  --------------------------//

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &os,
     multi_delimitator<Ch> const &d );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &is,
     multi_delimitator<Ch> const &d );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &os,
     multi_newer const &n );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &is,
     multi_skipper const &s );


//  Implementation detail stuff  ---------------------------------------------//

namespace detail
{

    // Base class (template) for repeated character streaming
    template < typename Ch = char >
    class repeated_character_streamer_base
    {
    public:
        // Template argument
        typedef Ch  char_type;

        // Lifetime management
        repeated_character_streamer_base( char_type c, ::std::streamsize count,
         bool synchronize_afterwards )
            : c_( c ), count_( count ), sync_( synchronize_afterwards )
            {}

        // Accessors
        char_type          repeated_char() const { return this->c_; }
        ::std::streamsize  repeat_count() const { return this->count_; }

        bool  will_synchronize_afterwards() const { return this->sync_; }

    private:
         // Member data
        char_type          c_;
        ::std::streamsize  count_;
        bool               sync_;

    };  // boost::io::detail::repeated_character_streamer_base

}  // namespace detail


//  I/O-manipulator object class (template) declarations  --------------------//

template < typename Ch >
class multi_delimitator
    : public detail::repeated_character_streamer_base<Ch>
{
    typedef detail::repeated_character_streamer_base<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    multi_delimitator( char_type c, std::streamsize count,
     bool final_sync = false );

    // Operators
    template < class Tr >
    void  operator ()( ::std::basic_ostream<Ch, Tr> &os ) const;

    template < class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::multi_delimitator

class multi_newer
    : public detail::repeated_character_streamer_base<>
{
    typedef detail::repeated_character_streamer_base<>  base_type;

public:
    // Lifetime management
    multi_newer( char c, ::std::streamsize count, bool final_flush = false );

    // Operators
    template < typename Ch, class Tr >
    void  operator ()( ::std::basic_ostream<Ch, Tr> &os ) const;

};  // boost::io::multi_newer

class multi_skipper
    : public detail::repeated_character_streamer_base<>
{
    typedef detail::repeated_character_streamer_base<>  base_type;

public:
    // Lifetime management
    multi_skipper( char c, ::std::streamsize count, bool final_sync = false );

    // Operators
    template < typename Ch, class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::multi_skipper


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
std::basic_ostream<Ch, Tr> &
newl
(
    std::basic_ostream<Ch, Tr> &  os
)
{
    return os << multi_newl( 1 );
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


//  I/O-manipulator object class (template) member function definitions  -----//

template < typename Ch >
inline
multi_delimitator<Ch>::multi_delimitator
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
multi_delimitator<Ch>::operator ()
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

template < typename Ch >
template < class Tr >
inline
void
multi_delimitator<Ch>::operator ()
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
    os << multi_delimitator<Ch>( os.widen(this->repeated_char()),
     this->repeat_count(), this->will_synchronize_afterwards() );
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
    is >> multi_delimitator<Ch>( is.widen(this->repeated_char()),
     this->repeat_count(), this->will_synchronize_afterwards() );
}


//  I/O-manipulator operator function definitions  ---------------------------//

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
operator <<
(
    std::basic_ostream<Ch, Tr> &   os,
    multi_delimitator<Ch> const &  d
)
{
    return d( os ), os;
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &   is,
    multi_delimitator<Ch> const &  d
)
{
    return d( is ), is;
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


#endif  // BOOST_IO_IOMANIP_HPP

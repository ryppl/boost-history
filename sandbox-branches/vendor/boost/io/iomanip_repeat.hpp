//  Boost io/iomanip_repeat.hpp header file  ---------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_REPEAT_HPP
#define BOOST_IO_IOMANIP_REPEAT_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/limits.hpp>  // for std::numeric_limits

#include <ios>      // for std::streamsize
#include <istream>  // for std::basic_istream
#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < typename Ch >
    class repeat_ch;

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &s,
     repeat_ch<Ch> const &r );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &s,
     repeat_ch<Ch> const &r );

class repeat_char;

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &s,
     repeat_char const &r );

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &s,
     repeat_char const &r );


//  Implementation detail stuff  ---------------------------------------------//

namespace detail
{

template < typename Ch >
class repeat_char_base
{
public:
    // Template argument
    typedef Ch  char_type;

    // Accessors
    char_type          repeated_char() const { return this->c_; }
    ::std::streamsize  repeat_count() const { return this->count_; }

    bool  will_synchronize_afterwards() const { return this->sync_; }

protected:
    // Lifetime management
    repeat_char_base( char_type c, ::std::streamsize count,
     bool synchronize_afterwards )
        : c_( c ), count_( count ), sync_( synchronize_afterwards )
        {}

private:
     // Member data
    char_type          c_;
    ::std::streamsize  count_;
    bool               sync_;

};  // boost::io::detail::repeat_char_base

}  // namespace detail


//  Repeated-character I/O manipulator class template declaration  -----------//

template < typename Ch >
class repeat_ch
    : private detail::repeat_char_base< Ch >
{
    typedef detail::repeat_char_base<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    repeat_ch( char_type c, ::std::streamsize count,
     bool synchronize_afterwards = false );

    // Accessors
    using base_type::repeated_char;
    using base_type::repeat_count;
    using base_type::will_synchronize_afterwards;

    // Operators
    template < class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

    template < class Tr >
    void  operator ()( ::std::basic_ostream<Ch, Tr> &os ) const;

};  // boost::io::repeat_ch


//  Specific repeated-character I/O manipulator class declaration  -----------//

class repeat_char
    : private detail::repeat_char_base< char >
{
    typedef detail::repeat_char_base<char>  base_type;

public:
    // Types
    typedef char  char_type;

    // Lifetime management
    repeat_char( char_type c, ::std::streamsize count,
     bool synchronize_afterwards = false );

    // Accessors
    using base_type::repeated_char;
    using base_type::repeat_count;
    using base_type::will_synchronize_afterwards;

    // Operators
    template < typename Ch, class Tr >
    void  operator ()( ::std::basic_istream<Ch, Tr> &is ) const;

    template < typename Ch, class Tr >
    void  operator ()( ::std::basic_ostream<Ch, Tr> &os ) const;

};  // boost::io::repeat_char


//  Repeated-character I/O-manipulator member function definitions  ----------//

template < typename Ch >
inline
repeat_ch<Ch>::repeat_ch
(
    char_type          c,
    ::std::streamsize  count,
    bool               synchronize_afterwards  // = false
)
    : base_type( c, count, synchronize_afterwards )
{
}

template < typename Ch >
template < class Tr >
void
repeat_ch<Ch>::operator ()
(
    ::std::basic_istream<Ch, Tr> &  is
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

template < typename Ch >
template < class Tr >
void
repeat_ch<Ch>::operator ()
(
    ::std::basic_ostream<Ch, Tr> &  os
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


//  Specific repeated-character I/O-manipulator member function definitions  -//

inline
repeat_char::repeat_char
(
    char_type          c,
    ::std::streamsize  count,
    bool               synchronize_afterwards  // = false
)
    : base_type( c, count, synchronize_afterwards )
{
}

template < typename Ch, class Tr >
inline
void
repeat_char::operator ()
(
    ::std::basic_istream<Ch, Tr> &  is
) const
{
    is >> repeat_ch<Ch>( is.widen(this->repeated_char()), this->repeat_count(),
     this->will_synchronize_afterwards() );
}

template < typename Ch, class Tr >
inline
void
repeat_char::operator ()
(
    ::std::basic_ostream<Ch, Tr> &  os
) const
{
    os << repeat_ch<Ch>( os.widen(this->repeated_char()), this->repeat_count(),
     this->will_synchronize_afterwards() );
}


//  I/O-manipulator operator function definitions  ---------------------------//

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  s,
    repeat_ch<Ch> const &         r
)
{
    return r( s ), s;
}

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
operator <<
(
    std::basic_ostream<Ch, Tr> &  s,
    repeat_ch<Ch> const &         r
)
{
    return r( s ), s;
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  s,
    repeat_char const &           r
)
{
    return r( s ), s;
}

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
operator <<
(
    std::basic_ostream<Ch, Tr> &  s,
    repeat_char const &           r
)
{
    return r( s ), s;
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_REPEAT_HPP

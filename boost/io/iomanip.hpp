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


//  I/O-manipulator function template declarations  --------------------------//

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  newl( std::basic_ostream<Ch, Tr> &os );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  skipl( std::basic_istream<Ch, Tr> &is );

template < typename Ch, class Tr >
    std::basic_ios<Ch, Tr> &  resetios( std::basic_ios<Ch, Tr> &s );


//  I/O-manipulator operator function declarations  --------------------------//

template < typename Ch, class Tr >
    std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &os,
     multi_newl const &n );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &is,
     multi_skipl const &s );


//  I/O-manipulator object class template declarations  ----------------------//

class multi_newl
{
public:
    // Lifetime management
    explicit  multi_newl( std::streamsize count, bool final_flush = false );

    // Operators
    template < typename Ch, class Tr >
    void  operator ()( std::basic_ostream<Ch, Tr> &os ) const;

private:
    // Member data
    std::streamsize  count_;
    bool             flush_;

};  // boost::io::multi_newl

class multi_skipl
{
public:
    // Lifetime management
    explicit  multi_skipl( std::streamsize count, bool final_sync = false );

    // Operators
    template < typename Ch, class Tr >
    void  operator ()( std::basic_istream<Ch, Tr> &is ) const;

private:
    // Member data
    std::streamsize  count_;
    bool             sync_;

};  // boost::io::multi_skipl


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


//  I/O-manipulator object class template member function definitions  -------//

inline
multi_newl::multi_newl
(
    std::streamsize  count,
    bool             final_flush  // = false
)
    : count_( count ), flush_( final_flush )
{
}

template < typename Ch, class Tr >
inline
void
multi_newl::operator ()
(
    std::basic_ostream<Ch, Tr> &  os
) const
{
    Ch const  new_line = os.widen( '\n' );

    for ( std::streamsize i = this->count_ ; (i > 0) && os ; --i )
    {
        os.put( new_line );
    }

    if ( this->flush_ && os )
    {
        os.flush();
    }
}

inline
multi_skipl::multi_skipl
(
    std::streamsize  count,
    bool             final_sync  // = false
)
    : count_( count ), sync_( final_sync )
{
}

template < typename Ch, class Tr >
inline
void
multi_skipl::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    using std::streamsize;

    typedef typename Tr::int_type  int_type;

    int_type const  new_line_int = Tr::to_int_type( is.widen('\n') );

    for ( streamsize i = this->count_ ; (i > 0) && is ; --i )
    {
        is.ignore( std::numeric_limits<streamsize>::max(), new_line_int );
    }

    if ( this->sync_ && is )
    {
        is.sync();
    }
}


//  I/O-manipulator operator function definitions  ---------------------------//

template < typename Ch, class Tr >
inline
std::basic_ostream<Ch, Tr> &
operator <<
(
    std::basic_ostream<Ch, Tr> &  os,
    multi_newl const &            n
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
    multi_skipl const &           s
)
{
    return s( is ), is;
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_HPP

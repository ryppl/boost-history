//  Boost io/iomanip.hpp header file  ----------------------------------------//

//  Copyright 2003-2004 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_HPP
#define BOOST_IO_IOMANIP_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/io/ios_state.hpp>  // for boost::io::ios_flags_saver, etc.
#include <boost/limits.hpp>        // for std::numeric_limits

#include <ios>      // for std::streamsize, std::basic_ios, std::ios_base
#include <istream>  // for std::basic_istream
#include <ostream>  // for std::basic_ostream
#include <string>   // for std::char_traits


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
     multi_basic_newer<Ch> const &n );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &is,
     multi_basic_skipper<Ch> const &s );

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

        // Accessors
        char_type          repeated_char() const { return this->c_; }
        ::std::streamsize  repeat_count() const { return this->count_; }

        bool  will_synchronize_afterwards() const { return this->sync_; }

    protected:
        // Lifetime management
        repeated_character_streamer_base( char_type c, ::std::streamsize count,
         bool synchronize_afterwards )
            : c_( c ), count_( count ), sync_( synchronize_afterwards )
            {}

    private:
         // Member data
        char_type          c_;
        ::std::streamsize  count_;
        bool               sync_;

    };  // boost::io::detail::repeated_character_streamer_base

}  // namespace detail


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


//  Form-based I/O manipulator class template and operator declarations  -----//

template < typename Ch, class Tr >
class basic_ios_form
{
    typedef basic_ios_form<Ch, Tr>  self_type;

    template < typename Obj >  struct binder_out;
    template < typename Obj >  struct binder_inout;

public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Lifetime management (use automatic destructor and copy constructor)
              basic_ios_form();
    explicit  basic_ios_form( ::std::basic_ios<Ch, Tr> const &i );

    // Accessors
    bool  override_fill() const;
    bool  override_precision() const;
    bool  override_width() const;

    ::std::ios_base::fmtflags  overridden_flags() const;
    bool                       override_flags() const;

    bool  override_adjustfield() const;
    bool  override_basefield() const;
    bool  override_floatfield() const;

    bool  override_boolalpha() const;
    bool  override_showbase() const;
    bool  override_showpoint() const;
    bool  override_showpos() const;
    bool  override_skipws() const;
    bool  override_unitbuf() const;
    bool  override_uppercase() const;

    char_type                  fill() const;
    ::std::streamsize          precision() const;
    ::std::streamsize          width() const;
    ::std::ios_base::fmtflags  flags() const;

    // Mutators
    self_type &  fill( char_type const &c );

    self_type &  precision( ::std::streamsize s );
    self_type &  width( ::std::streamsize s );

    self_type &    flag( ::std::ios_base::fmtflags f );
    self_type &  unflag( ::std::ios_base::fmtflags f );
    self_type &    flags( ::std::ios_base::fmtflags f,
     ::std::ios_base::fmtflags mask );

    self_type &  boolalpha();
    self_type &  noboolalpha();

    self_type &  showbase();
    self_type &  noshowbase();

    self_type &  showpoint();
    self_type &  noshowpoint();

    self_type &  showpos();
    self_type &  noshowpos();

    self_type &  skipws();
    self_type &  noskipws();

    self_type &  unitbuf();
    self_type &  nounitbuf();

    self_type &  uppercase();
    self_type &  nouppercase();

    self_type &  internal();
    self_type &  left();
    self_type &  right();

    self_type &  dec();
    self_type &  hex();
    self_type &  oct();

    self_type &  fixed();
    self_type &  scientific();

    // Operators
    template < typename Obj >
    binder_out<Obj>  operator ()( Obj const &o ) const
    { return binder_out<Obj>( *this, o ); }

    template < typename Obj >
    binder_inout<Obj>  operator ()( Obj &o ) const
    { return binder_inout<Obj>( *this, o ); }

private:
    // Member templates
    template < typename Obj >
    struct binder_out
    {
        self_type const &  f_;
        Obj const &        o_;

        binder_out( self_type const &f, Obj const &o )
            : f_( f ), o_( o )
            {}

        friend
        ::std::basic_ostream<Ch, Tr> &
        operator <<( ::std::basic_ostream<Ch, Tr> &os, binder_out const &b )
        {
            ios_flags_saver      fs( os, b.f_.override_flags() ? (( b.f_.flags()
             & b.f_.overridden_flags() ) | ( os.flags()
             & ~b.f_.overridden_flags() )) : os.flags() );
            ios_precision_saver  ps( os, b.f_.override_precision()
             ? b.f_.precision() : os.precision() );
            ios_width_saver      ws( os, b.f_.override_width() ? b.f_.width()
             : os.width() );

            basic_ios_fill_saver<Ch, Tr>  bfs( os, b.f_.override_fill()
             ? b.f_.fill() : os.fill() );

            return os << b.o_;
        }

    };  // boost::io::basic_ios_form::binder_out

    template < typename Obj >
    struct binder_inout
    {
        self_type const &  f_;
        Obj &              o_;

        binder_inout( self_type const &f, Obj &o )
            : f_( f ), o_( o )
            {}

        friend
        ::std::basic_ostream<Ch, Tr> &
        operator <<( ::std::basic_ostream<Ch, Tr> &os, binder_inout const &b )
        {
            ios_flags_saver      fs( os, b.f_.override_flags() ? (( b.f_.flags()
             & b.f_.overridden_flags() ) | ( os.flags()
             & ~b.f_.overridden_flags() )) : os.flags() );
            ios_precision_saver  ps( os, b.f_.override_precision()
             ? b.f_.precision() : os.precision() );
            ios_width_saver      ws( os, b.f_.override_width() ? b.f_.width()
             : os.width() );

            basic_ios_fill_saver<Ch, Tr>  bfs( os, b.f_.override_fill()
             ? b.f_.fill() : os.fill() );

            return os << b.o_;
        }

        friend
        ::std::basic_istream<Ch, Tr> &
        operator >>( ::std::basic_istream<Ch, Tr> &is, binder_inout const &b )
        {
            ios_flags_saver      fs( is, b.f_.override_flags() ? (( b.f_.flags()
             & b.f_.overridden_flags() ) | ( is.flags()
             & ~b.f_.overridden_flags() )) : is.flags() );
            ios_precision_saver  ps( is, b.f_.override_precision()
             ? b.f_.precision() : is.precision() );
            ios_width_saver      ws( is, b.f_.override_width() ? b.f_.width()
             : is.width() );

            basic_ios_fill_saver<Ch, Tr>  bfs( is, b.f_.override_fill()
             ? b.f_.fill() : is.fill() );

            return is >> b.o_;
        }

    };  // boost::io::basic_ios_form::binder_inout

    // Member data
    char_type                  fill_;
    ::std::streamsize          precision_, width_;
    ::std::ios_base::fmtflags  flags_;

    bool                       use_fill_, use_precision_, use_width_;
    ::std::ios_base::fmtflags  used_flags_;

};  // boost::io::basic_ios_form


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


//  Form-based I/O manipulator class template and operator definitions  ------//

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr>::basic_ios_form
(
)
    : fill_(), precision_( 6 ), width_( 0 )
    , flags_( ::std::ios_base::skipws | ::std::ios_base::dec )
    , use_fill_( false ), use_precision_( false ), use_width_( false )
    , used_flags_( ::std::ios_base::fmtflags() )
{
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr>::basic_ios_form
(
    ::std::basic_ios<Ch, Tr> const &  i
)
    : fill_( i.fill() ), precision_( i.precision() ), width_( i.width() )
    , flags_( i.flags() )
    , use_fill_( true ), use_precision_( true ), use_width_( true )
    , used_flags_( ~( ::std::ios_base::fmtflags() ) )
{
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_fill
(
) const
{
    return this->use_fill_;
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_precision
(
) const
{
    return this->use_precision_;
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_width
(
) const
{
    return this->use_width_;
}

template < typename Ch, class Tr >
inline
::std::ios_base::fmtflags
basic_ios_form<Ch, Tr>::overridden_flags
(
) const
{
    return this->used_flags_;
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_flags
(
) const
{
    return ::std::ios_base::fmtflags() != this->used_flags_;
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_adjustfield
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::adjustfield );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_basefield
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::basefield );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_floatfield
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::floatfield );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_boolalpha
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::boolalpha );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_showbase
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::showbase );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_showpoint
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::showpoint );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_showpos
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::showpos );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_skipws
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::skipws );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_unitbuf
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::unitbuf );
}

template < typename Ch, class Tr >
inline
bool
basic_ios_form<Ch, Tr>::override_uppercase
(
) const
{
    return ::std::ios_base::fmtflags() != ( this->used_flags_
     & ::std::ios_base::uppercase );
}

template < typename Ch, class Tr >
inline
typename basic_ios_form<Ch, Tr>::char_type
basic_ios_form<Ch, Tr>::fill
(
) const
{
    return this->fill_;
}

template < typename Ch, class Tr >
inline
::std::streamsize
basic_ios_form<Ch, Tr>::precision
(
) const
{
    return this->precision_;
}

template < typename Ch, class Tr >
inline
::std::streamsize
basic_ios_form<Ch, Tr>::width
(
) const
{
    return this->width_;
}

template < typename Ch, class Tr >
inline
::std::ios_base::fmtflags
basic_ios_form<Ch, Tr>::flags
(
) const
{
    return this->flags_;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::fill
(
    typename basic_ios_form<Ch, Tr>::char_type const &  c
)
{
    traits_type::assign( this->fill_, c );
    this->use_fill_ = true;
    return *this;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::precision
(
    ::std::streamsize  s
)
{
    this->precision_ = s;
    this->use_precision_ = true;
    return *this;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::width
(
    ::std::streamsize  s
)
{
    this->width_ = s;
    this->use_width_ = true;
    return *this;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::flag
(
    ::std::ios_base::fmtflags  f
)
{
    this->flags_ |= f;
    this->used_flags_ |= f;
    return *this;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::unflag
(
    ::std::ios_base::fmtflags  f
)
{
    this->flags_ &= ~f;
    this->used_flags_ |= f;
    return *this;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::flags
(
    ::std::ios_base::fmtflags  f,
    ::std::ios_base::fmtflags  mask
)
{
    this->flags_ &= ~mask;
    this->flags_ |= ( f & mask );
    this->used_flags_ |= mask;
    return *this;
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::boolalpha
(
)
{
    return this->flag( ::std::ios_base::boolalpha );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::noboolalpha
(
)
{
    return this->unflag( ::std::ios_base::boolalpha );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::showbase
(
)
{
    return this->flag( ::std::ios_base::showbase );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::noshowbase
(
)
{
    return this->unflag( ::std::ios_base::showbase );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::showpoint
(
)
{
    return this->flag( ::std::ios_base::showpoint );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::noshowpoint
(
)
{
    return this->unflag( ::std::ios_base::showpoint );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::showpos
(
)
{
    return this->flag( ::std::ios_base::showpos );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::noshowpos
(
)
{
    return this->unflag( ::std::ios_base::showpos );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::skipws
(
)
{
    return this->flag( ::std::ios_base::skipws );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::noskipws
(
)
{
    return this->unflag( ::std::ios_base::skipws );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::unitbuf
(
)
{
    return this->flag( ::std::ios_base::unitbuf );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::nounitbuf
(
)
{
    return this->unflag( ::std::ios_base::unitbuf );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::uppercase
(
)
{
    return this->flag( ::std::ios_base::uppercase );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::nouppercase
(
)
{
    return this->unflag( ::std::ios_base::uppercase );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::internal
(
)
{
    return this->flags( ::std::ios_base::internal, ::std::ios_base::adjustfield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::left
(
)
{
    return this->flags( ::std::ios_base::left, ::std::ios_base::adjustfield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::right
(
)
{
    return this->flags( ::std::ios_base::right, ::std::ios_base::adjustfield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::dec
(
)
{
    return this->flags( ::std::ios_base::dec, ::std::ios_base::basefield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::hex
(
)
{
    return this->flags( ::std::ios_base::hex, ::std::ios_base::basefield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::oct
(
)
{
    return this->flags( ::std::ios_base::oct, ::std::ios_base::basefield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::fixed
(
)
{
    return this->flags( ::std::ios_base::fixed, ::std::ios_base::floatfield );
}

template < typename Ch, class Tr >
inline
basic_ios_form<Ch, Tr> &
basic_ios_form<Ch, Tr>::scientific
(
)
{
    return this->flags( ::std::ios_base::scientific, ::std::ios_base::floatfield );
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_HPP
